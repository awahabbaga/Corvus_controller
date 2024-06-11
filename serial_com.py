import serial
import time

def send_command(ser, command):
    """
    Send a command to the Corvus controller.
    
    :param ser: Serial object
    :param command: Command string to send
    """
    ser.write(f"{command}\r\n".encode())
    time.sleep(0.1)  # Wait for the command to be processed

def read_response(ser):
    """
    Read the response from the Corvus controller.
    
    :param ser: Serial object
    :return: Response string
    """
    response = ser.readline().decode().strip()
    return response

def move(ser, coordinates):
    """
    Send the 'move' command to the Corvus controller with the specified coordinates.
    The coordinates are converted to micrometers before sending.
    
    :param ser: Serial object
    :param coordinates: String of space-separated coordinates in millimeters
    """
    coordinates_mm = coordinates.split()
    coordinates_um = [str(float(coord) * 1000) for coord in coordinates_mm]
    command = " ".join(coordinates_um) + " move"
    send_command(ser, command)
    response = read_response(ser)
    print(f"Response: {response}")

def main():
    # Replace '/dev/ttyUSB0' with the appropriate serial port for your setup
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    time.sleep(2)  # Wait for the serial connection to initialize

    try:
        while True:
            command = input("Enter 'move' command coordinates in mm (space-separated) or type 'exit' to quit: ")
            if command.lower() == 'exit':
                break
            
            move(ser, command)

    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        ser.close()

if __name__ == "__main__":
    main()
