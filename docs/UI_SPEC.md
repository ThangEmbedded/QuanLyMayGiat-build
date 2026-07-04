# UI_SPEC

## Home

Machine card shows:

- Machine ID
- Room
- Status ring

Status ring:
- Gray = Empty
- Blue = Running
- Yellow = Finished

Running text:
- Đang giặt
- Đã giặt XX phút

No remaining countdown.

## Start Flow

Empty Machine
-> Room Input
-> Numeric Keypad
-> Confirm
-> Relay ON
-> Running

## Admin

- Top-right button
- Large touch area
- No default password hint
- Wrong password:
  - Popup
  - Auto return HOME after 5s

## Numeric Keypad

Use for:
- Room
- Password

Do not use OS virtual keyboard.
