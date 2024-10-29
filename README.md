# PintOS Project: Solving Busy Wait with Alarm Clock

## Description

This project enhances PintOS by replacing busy wait loops with an alarm clock mechanism, reducing CPU usage and improving system efficiency by allowing processes to sleep until needed without consuming resources.

### Modified Files and Functions
- **`threads/thread.c`**:
  - **Functions**:
    - `thread_init()` 
- **`devices/timer.c`**:
  - **Functions**:
    - `timer_sleep()`
    - `timer_interrupt()`

## Testing on Pint-OS after compiling the code with changes
   ```bash
   pintos -- -q run alarm-multiple
   ```

