import cv2
import mediapipe as mp
from flask import Flask, Response, render_template, request
from sound import *

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

class VideoCamera(object):
    def __init__(self, video_path):
        self.video = cv2.VideoCapture(video_path, cv2.CAP_DSHOW)

    def __del__(self):
        self.video.release()

    def get_frame(self):
        with mp_hands.Hands(model_complexity=0, min_detection_confidence=0.5, min_tracking_confidence=0.5) as hands:
            success, image = self.video.read()

            # To improve performance, optionally mark the image as not writeable to
            # pass by reference.
            image.flags.writeable = False
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            results = hands.process(image)

            # Draw the hand annotations on the image.
            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
            if results.multi_hand_landmarks:
                for hand_landmarks in results.multi_hand_landmarks:
                    mp_drawing.draw_landmarks(
                        image,
                        hand_landmarks,
                        mp_hands.HAND_CONNECTIONS,
                        mp_drawing_styles.get_default_hand_landmarks_style(),
                        mp_drawing_styles.get_default_hand_connections_style())

            _, jpeg = cv2.imencode('.jpg', image)
            return jpeg.tobytes()

def gen(camera):
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')


app = Flask(__name__)

@app.route('/')
def index():
    """Mediapipe streaming home page."""
    return render_template('index.html')

@app.route('/home')
def home():
    return render_template('home.html')

@app.route('/camera')
def camera():
    return Response(gen(VideoCamera(0)), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/sound')
def sound():
    # return Response(serial_start())
    return serial_start()
    # return render_template('sound.html')

@app.route('/practice')
def practice():
    return render_template('practice.html')

@app.route('/carousel')
def carousel():
    return render_template('carousel.html')


if __name__ == '__main__':
    app.run(debug=True)