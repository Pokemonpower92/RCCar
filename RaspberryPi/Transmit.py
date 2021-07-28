import time
from rf24libs import RF24
from RF24 import RF24, RF24_PA_LOW

radio = RF24(25, 0)
send = None
address = b'00001'

if __name__ == '__main__':
    
    if not radio.begin():
        print("Radio is borked")

    x = '0512'
    y = '0512' 
    s = 'T'

    # Package up the message and send it over. 
    send = s + '.' + x + '.' + y
    radio.setPALevel(RF24_PA_LOW)
    radio.openWritingPipe(address)
    radio.payloadSize = len(send) + 1
    radio.stopListening()
    while True:
        if radio.write(send.encode()):
            print("Wrote to receiver.")
            print("Sent: ", send)
        else:
            print("Message lost.")
        
        print()
        time.sleep(1)