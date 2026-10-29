# STM32 Reaction Game

A reaction-time game developed in C for the **STM32 NUCLEO-F401RE** development board using STM32CubeIDE and STM32CubeMX.

The goal of this project is to develop and consolidate embedded systems programming skills through a practical application involving GPIO, hardware timers, interrupts, UART communication, and modular software architecture.

The project will feature two implementations of the same game, allowing a comparison between conventional control flow and a Finite State Machine (FSM).

## Table of Contents

- [STM32 Reaction Game](#stm32-reaction-game)
  - [Table of Contents](#table-of-contents)
  - [Project Overview](#project-overview)
    - [Learning Objectives](#learning-objectives)
  - [Hardware](#hardware)
  - [Game Workflow](#game-workflow)
  - [Functional Requirements](#functional-requirements)
  - [Peripheral Configuration](#peripheral-configuration)
    - [System Clock](#system-clock)
    - [TIM2 — Reaction-Time Measurement](#tim2--reaction-time-measurement)
    - [TIM3 — Random Waiting Interval](#tim3--random-waiting-interval)
    - [GPIO — LED and User Button](#gpio--led-and-user-button)
    - [USART2 — UART Communication](#usart2--uart-communication)
  - [Software Architecture](#software-architecture)
    - [Common Modules](#common-modules)
    - [Direct Implementation](#direct-implementation)
    - [Finite State Machine Implementation](#finite-state-machine-implementation)
  - [Git Branch Strategy](#git-branch-strategy)
  - [Development Status](#development-status)
    - [Completed](#completed)
    - [Pending](#pending)
  - [Development Environment](#development-environment)
  - [Serial Terminal](#serial-terminal)

## Project Overview

The STM32 Reaction Game measures how quickly a user reacts to a visual signal.

The game uses the development board's built-in LED and user button. Reaction times are transmitted to a computer through UART and displayed in a serial terminal.

No additional electronic components are required beyond the development board and a USB connection to the computer.

### Learning Objectives

- Configure and control GPIO peripherals.
- Use hardware timers for time measurement and periodic events.
- Implement interrupt-driven programming.
- Communicate with a computer through UART.
- Develop modular firmware using C source (`.c`) and header (`.h`) files.
- Use Git branches to maintain independent implementations.
- Design and implement a Finite State Machine for an embedded application.

## Hardware

| Component | Description |
|---|---|
| Development board | STM32 NUCLEO-F401RE |
| Microcontroller | STM32F401RE |
| Visual indicator | On-board LED (LD2) |
| User input | On-board user button (B1) |
| Computer interface | USB connection through the ST-LINK virtual COM port |

No external hardware components are required.

## Game Workflow

The intended game sequence is:

1. The firmware starts the game automatically after initialization.
2. The system waits for a random time interval.
3. The on-board LED turns on to signal the user.
4. The user presses the on-board button as quickly as possible.
5. The microcontroller measures the reaction time.
6. The result is transmitted to the computer through UART.
7. A new round starts automatically.

The random waiting interval prevents the user from anticipating when the LED will turn on.

## Functional Requirements

The firmware implements the following features:

- **Automatic startup:** The game starts without requiring an initial command.
- **Random delay:** A random waiting interval precedes the visual signal.
- **LED indication:** The built-in LED indicates when the user should react.
- **Button detection:** The built-in button registers the user's response.
- **Reaction-time measurement:** Hardware timer counting measures the elapsed time between the LED turning on and the button press.
- **UART output:** Reaction-time results are transmitted to a computer and displayed in a serial terminal.
- **Automatic replay:** A new round starts after the previous result has been reported.

## Peripheral Configuration

The initial peripheral configuration is generated using STM32CubeMX and integrated into the STM32CubeIDE project.

### System Clock

The system clock is configured using the internal High-Speed Internal (HSI) oscillator and the Phase-Locked Loop (PLL).

| Parameter | Value |
|---|---|
| HSI frequency | 16 MHz |
| PLL configuration | M = 16, N = 336, P = 4 |
| SYSCLK frequency | 84 MHz |
| AHB prescaler | DIV1 |
| APB1 prescaler | DIV2 |
| APB1 peripheral clock | 42 MHz |
| APB1 timer clock | 84 MHz |

The APB1 timers receive an 84 MHz clock because the APB1 prescaler is greater than one.

### TIM2 — Reaction-Time Measurement

TIM2 is configured as a hardware time base for measuring the user's reaction time.

| Parameter | Value |
|---|---|
| Timer clock | 84 MHz |
| Prescaler (PSC) | 41999 |
| Auto-reload value (ARR) | 999 |
| Counter mode | Up-counting |
| Counter increment | 0.5 ms |
| Update period | 500 ms |
| NVIC interrupt | Enabled |

With this configuration, the timer counter increments every 0.5 ms and generates an update interrupt every 500 ms.

The application uses TIM2 to measure reaction time. The `Arrancar()` function resets the timer and the software interrupt counter before starting the measurement.

When the user presses the button while the LED is on, `Obtener_Cuenta()` stops TIM2 and calculates the elapsed time using the number of timer interrupts and the current hardware counter value.

The calculated time is expressed in seconds.

### TIM3 — Random Waiting Interval

TIM3 is configured as a hardware time base for managing the waiting interval before the LED turns on.

| Parameter | Value |
|---|---|
| Timer clock | 84 MHz |
| Prescaler (PSC) | 8399 |
| Auto-reload value (ARR) | 9999 |
| Counter mode | Up-counting |
| Counter increment | 0.1 ms |
| Update period | 1 second |
| NVIC interrupt | Enabled |

With this configuration, the timer counter increments every 0.1 ms and generates an update interrupt every second.

The `Generate_Number()` function generates a random waiting interval between 1 and 5 seconds.

TIM3 counts these intervals through its update interrupts. When the software counter reaches the generated value, the firmware stops TIM3, resets its counters, turns on the LED, and starts the reaction-time measurement using TIM2.


**Note:** The timer configuration must match the actual values generated by STM32CubeMX. If the prescaler or auto-reload value changes, the timer update period must be recalculated accordingly.

### GPIO — LED and User Button

The built-in LED (LD2) is configured as a GPIO output.

The built-in user button (B1) is configured as an external interrupt input using a falling-edge trigger.

The corresponding external interrupt is enabled in the Nested Vectored Interrupt Controller (NVIC).

The led.c module provides functions to:

- Turn on the LED using `Encender()`.
- Turn off the LED using `Apagar()`.
- Read its current status using `Estado()`.

The button interrupt callback sets a software flag when a valid press is detected. The main loop processes this flag and determines whether the press should be accepted or ignored.

A 50 ms debounce interval is implemented using `HAL_GetTick()` to reduce the effect of button contact bouncing.

### USART2 — UART Communication

USART2 is configured to transmit game results to the computer through the board's USB virtual COM port.

| Parameter | Value |
|---|---|
| Baud rate | 115200 |
| Word length | 8 bits |
| Stop bits | 1 |
| Parity | None |
| Direction | Transmit only |
| Hardware flow control | Disabled |
| UART interrupts | Not required for the planned blocking transmission approach |

The uart_interface.c module provides the `Mensaje()` function to format and print the measured reaction time.

The application uses the `_write()` function to redirect standard output to USART2. This allows `printf()` messages to be transmitted to the serial terminal.

The output includes the measured reaction time in seconds.

## Software Architecture

The firmware will be organized into independent modules to separate hardware control, timing, communication, and game logic.

### Common Modules

The following modules are implemented or planned for both implementations:

| File | Responsibility |
|---|---|
| `main.c` | System initialization and main application loop |
| `led.c` / `led.h` | Control of the built-in LED |
| `reaction_timer.c` / `reaction_timer.h` | Reaction-time measurement |
| `random_delay.c` / `random_delay.h` | Random waiting-interval management |
| `uart_interface.c` / `uart_interface.h` | UART communication and result transmission |
| `callbacks.c` | Peripheral interrupt callback handling |

The exact responsibilities and interfaces of these modules will be refined during implementation.

### Direct Implementation

The `direct-implementation` branch will implement the game logic using conventional control-flow structures.

The game sequence will be managed directly by the application code without a dedicated FSM module.

This implementation will provide a straightforward approach for comparison with the state-machine version.

### Finite State Machine Implementation

The `state-machine` branch will implement the game logic using a Finite State Machine (FSM).

The game will be divided into explicit states representing its different stages. Transitions between states will be determined by timer events and button presses.

This implementation will introduce two additional files:

- `game_fsm.c`
- `game_fsm.h`

These files will exist exclusively in the `state-machine` branch.

The FSM approach will make the game flow explicit and provide an opportunity to study state-based software design in embedded systems.

## Git Branch Strategy

The project will be maintained in a single Git repository with three main branches.

| Branch | Purpose |
|---|---|
| `main` | Initial hardware configuration and generated firmware project |
| `direct-implementation` | Game logic implemented using conventional control flow |
| `state-machine` | Game logic implemented using a Finite State Machine |

Both implementation branches will originate from `main`, ensuring that they share the same initial hardware configuration.

The common hardware and communication modules will be maintained consistently across both implementations. The primary difference between the two branches will be the organization of the game logic.

This structure will allow both approaches to be developed and compared independently.

## Development Status

**Current stage: Direct implementation developed and undergoing hardware testing.**

### Completed

- STM32CubeMX peripheral configuration.
- System clock configuration.
- GPIO configuration for the built-in LED and user button.
- TIM2 configuration for reaction-time measurement.
- TIM3 configuration for waiting-interval timing.
- NVIC configuration for timer and button interrupts.
- USART2 configuration for serial transmission.
- Initial firmware code generation.
- Successful compilation of the generated project.
- Implementation of the common software modules.
- Random waiting-interval generation.
- Reaction-time measurement logic.
- Interrupt callback handling.
- UART result transmission.
- Direct game implementation.

### Pending

- FSM game implementation.
- Hardware testing and validation.

The current direct implementation is functional, with further testing and refinement still required.

## Development Environment

- **IDE:** STM32CubeIDE
- **Configuration tool:** STM32CubeMX
- **Programming language:** C
- **Version control:** Git
- **Repository hosting:** GitHub
- **Target board:** STM32 NUCLEO-F401RE

## Serial Terminal

The game sends reaction-time results to the computer through the board's USB virtual COM port.

On Linux, picocom can be used to open the serial terminal.

First, connect the development board through USB and identify the serial device. In the current setup, the device is /dev/ttyACM0.

Run the following command:

```bash
picocom -b 115200 /dev/ttyACM0
```
The baud rate must match the USART2 configuration.

Once the terminal is open, the reaction-time results will appear when a round is completed.

To exit picocom, press Ctrl+A followed by Ctrl+X.