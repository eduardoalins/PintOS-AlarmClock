# PintOS Project: Solving Busy Wait with Alarm Clock

## Description

This project enhances PintOS by replacing busy wait loops with an alarm clock mechanism, reducing CPU usage and improving system efficiency by allowing processes to sleep until needed without consuming resources.

### Modified Files and Functions

- **`threads/thread.c`**:
  - **Functions**:
    - `void thread_add_sleep(int64_t time)`
    - `void thread_wakeup(int64_t tick)`
    - `thread_init ()`
  - **Lines**:
    - `static struct list sleep_list;`
      
- **`threads/thread.h`**:
  - **Lines**:
    - `void thread_add_sleep(int64_t time)`
    - `void thread_wakeup(int64_t tick);`
    - `struct thread`

- **`devices/timer.c`**:
  - **Functions**:
    - `timer_sleep (int64_t ticks)`
    - `timer_interrupt (struct intr_frame *args UNUSED)`

## Testing on Pint-OS after compiling the code with changes
   ```bash
   pintos -- -q run alarm-multiple
   ```

