import serial
import time
import threading
import paho.mqtt.client as mqtt
from flask import Flask, jsonify, request
from flask_cors import CORS

L1 = 50  #soglia 1
L2 = 80  #soglia 2
T1 = 5.0 #secondi sopra L1 per aprire 50%
T2 = 10.0 #timeout per UNCONNECTED

system_state = {
    "water_level": 0,
    "wcs_mode": "UNKNOWN",
    "valve_aperture": 0,
    "status": "NORMAL",
    "history": []
}

l1_start_time = None
last_tms_msg_time = time.time()
ser = None

try:
    ser = serial.Serial('COM3', 115200, timeout=1)
except Exception as e:
    print(f"Porta seriale assente: {e}")

def write_serial(cmd):
    if ser:
        ser.write((cmd + "\n").encode('utf-8'))
        print(f"-> SERIAL OUT: {cmd}")

def serial_thread():
    global system_state
    while True:
        if ser and ser.in_waiting > 0: #dati nel buffer
            try:
                line = ser.readline().decode('utf-8').strip()
                if line.startswith("STATE:"):
                    parts = line.split(',')
                    system_state["wcs_mode"] = parts[0].split(':')[1]
                    system_state["valve_aperture"] = int(parts[1].split(':')[1])
            except: pass
        time.sleep(0.1)

def core_logic_thread():
    global system_state, l1_start_time, last_tms_msg_time
    while True:
        now = time.time()
        
        #controllo connessione
        if now - last_tms_msg_time > T2:
            if system_state["status"] != "UNCONNECTED":
                system_state["status"] = "UNCONNECTED"
                write_serial("CMD:STATE:UNCONNECTED")
        else:
            if system_state["status"] != "NORMAL":
                system_state["status"] = "NORMAL"
                write_serial("CMD:STATE:CONNECTED")

        #logica valvola auto
        if system_state["wcs_mode"] == "AUTO" and system_state["status"] == "NORMAL":
            level = system_state["water_level"]
            
            if level > L2:
                write_serial("CMD:VALVE:100")
                l1_start_time = None
            elif level > L1:
                if l1_start_time is None:
                    l1_start_time = now
                elif now - l1_start_time >= T1:
                    write_serial("CMD:VALVE:50")
            else:
                write_serial("CMD:VALVE:0")
                l1_start_time = None
                
        time.sleep(1)

def on_connect(client, userdata, flags, rc):
    client.subscribe("smart_tank/level")

def on_message(client, userdata, msg):
    global system_state, last_tms_msg_time
    try:
        level = int(msg.payload.decode())
        system_state["water_level"] = level
        last_tms_msg_time = time.time()
        
        #dati grafico
        system_state["history"].append({"time": time.strftime('%H:%M:%S'), "level": level})
        if len(system_state["history"]) > 20:
            system_state["history"].pop(0)
    except: pass

app = Flask(__name__)
CORS(app)

@app.route('/api/state', methods=['GET'])
def get_state():
    return jsonify(system_state)

@app.route('/api/command', methods=['POST'])
def send_command():
    data = request.json
    if "mode" in data:
        mode = "AUTO" if data["mode"] == "AUTO" else "MANUAL"
        write_serial(f"CMD:MODE:{mode}")
    if "valve" in data and system_state["wcs_mode"] == "MANUAL":
        write_serial(f"CMD:VALVE:{data['valve']}")
    return jsonify({"status": "ok"})

if __name__ == '__main__':
    threading.Thread(target=serial_thread, daemon=True).start()
    threading.Thread(target=core_logic_thread, daemon=True).start()
    
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect("broker.hivemq.com", 1883, 60)
    client.loop_start()
    
    app.run(host='0.0.0.0', port=5000, debug=False)