import RPi.GPIO as GPIO
from flask import Flask, render_template, request
app = Flask(__name__)

# 불필요한 warning 제거, GPIO 핀 번호 모드 설정
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

# pins라는 딕셔너리 만들고 핀 번호 저장
pins = {
    13 : {'name' : 'RED LED', 'state' : GPIO.LOW},
    16 : {'name' : 'YELLOW LED', 'state' : GPIO.LOW},
    12 : {'name' : 'GREEN LED', 'state' : GPIO.LOW},
}

# pins 내 모든 핀들을 출력으로 설정
for pin in pins:
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)

# 웹서버의 URL 주소로 접근하면 아래의 메인 함수 실행
@app.route("/")
def main():
    # pins 내 모든 핀의 현재 상태 업데이트
    for pin in pins:
        pins[pin]['state'] = GPIO.input(pin)

    # templateData에 저장
    templateData = {
        'pins' : pins
    }

    # 업데이트된 templateData 값들을 homeLED.html로 리턴
    return render_template('homeLED.html', **templateData)

# URL 주소 끝에 "/핀번호/<action>"을 붙여 접근 시 액션
@app.route("/<changePin>/<action>")
def action(changePin, action):
    # 현재 핀번호를 받은 번호로 설정
    changePin = int(changePin)

    # 핀 번호에 설정된 이름값 불러옴
    deviceName = pins[changePin]['name']

    # action 값이 'on'일 때
    if action == "on":
        GPIO.output(changePin, GPIO.HIGH)

    # action 값이 'off'일 때
    if action == "off":
        GPIO.output(changePin, GPIO.LOW)

    # GPIO ON/OFF 상태 저장
    pins[changePin]['state'] = GPIO.input(changePin)

    # 핀 값들을 업데이트해서 templateData에 저장
    templateData = {'pins' : pins}

    # 업데이트된 templateData 값을 리턴
    return render_template('homeLED.html', **templateData)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80, debug=False)