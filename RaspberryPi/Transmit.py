import time
from pynput import keyboard
from rf24libs import RF24
from RF24 import RF24, RF24_PA_LOW

radio = RF24(25, 0)
send = "F.0000.0000"
address = b'00001'



def onPress(key):
    x = '0000'
    y = '0000' 
    s = 'F'

    try: 
        if key == keyboard.Key.space:
            s = 'T'
        elif key.char == 'w':
            y = '0512'
        elif key.char == 's':
            y = '-512'
        elif key.char == 'a':
            x = '-512'
        elif key.char == 'd':
            x = '0512'
        else:
            x = '0000'
            y = '0000'
            s = 'F'
    except AttributeError:
        pass
    
    send = s + '.' + x + '.' + y

    if radio.write(send.encode()):
        print("Wrote to receiver.")
        print("Sent: ", send)
        time.sleep(.0001)
    else:
        print("Message lost.")
    
def onRelease(key):
    if key == keyboard.Key.esc:
        print("\nExiting controller")
        return False

if __name__ == '__main__':
    
    if not radio.begin():
        print("Radio is borked")

    
    radio.openWritingPipe(address)
    radio.payloadSize = len(send) + 1
    radio.stopListening()

    print("Python Car controller.")
    print("Forward:     w")
    print("Backward:    s")
    print("Left:        a")
    print("Right:       d")
    print("Special: space")
    print("Exit:      esc")

    # Get keyboard events from the user.
    with keyboard.Listener(on_press=onPress,
                           on_release=onRelease) as listener:
        listener.join()

    if radio.write(send.encode()):
        time.sleep(.0001)
    else:
        print("Message lost.")