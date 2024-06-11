# Corvus_controller
Sending Venus-1 commands to Corvus high resolution positioning controller by Serial (RS232) using high level programming langages (Python, C++)

## Project Structure

- `serial_com.py`: The program in python
- `com_serie.cpp`: The program in c++.

## Usage

1. Clone the repository:
    ```sh
    git clone https://github.com/awahabbaga/Corvus_controller.git
    cd Corvus_controller
    ```

2. Ensure all dependencies are installed:
    ```sh
    pip install -r requirements.txt
    ```
    - Make sure the serial port is connected.
    - Make sure you are on a  `Windows` system to be able to use the c++ program.
    - Specify the serial port and the baud rate in the code
    - Run the code either `com_serie.cpp` or `serial_com.py`

## Code Structure

- A function to send in the serial port `send_command` 
- A function to read the serial port `read_response`
- A dedicated function for the **move** command call `move`

## Venus-1 commands



