from flask import Flask, request, jsonify

app = Flask(__name__)

ESP8266_A_IP = "http://192.168.1.100"


@app.route('/request_data', )
def request_data():
    try:
        # request.get(f"{ESP8266_A_IP}/get_humidity_temperature")
        response = "Hello\json"
        data = response.json()
        return jsonify(response)
    except Exception as e:
        return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
