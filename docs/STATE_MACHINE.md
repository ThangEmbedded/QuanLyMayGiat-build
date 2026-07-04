# STATE_MACHINE

## Lifecycle

```text
EMPTY
  |
WAIT_CONFIRM
  |
RUNNING
  |
FINISHED
  |
EMPTY
```

### EMPTY
- Relay OFF
- Gray ring

### WAIT_CONFIRM
- Room input
- Numeric keypad
- Confirm / Cancel

### RUNNING
- Relay ON
- Blue blinking ring
- Show elapsed washing time

### FINISHED
- Relay OFF
- Yellow blink twice
- Hold yellow 10s
- Return EMPTY after 2 minutes

### Admin
HOME -> Login -> Success -> Setup
HOME -> Login -> Fail -> Popup 5s -> HOME
