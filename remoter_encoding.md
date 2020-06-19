# Encoding for TATUNG Air Conditioner Remoter -- CR-99DE

---

## Bits Encoding

When each button is pressed, the remoter would send a sequence of IR signals.  
Here we use following symbols to represent the IR signals.

- `^`: HIGH for 3850us, used in start codes
- `>`: LOW for 2175us, used in start codes
- `0`: HIGH for 850us then LOW for 375us, represent bit '0'
- `1`: HIGH for 850us then LOW for 1600us, represent bit '1'
- `E`: HIGH for 850us then LOW for 8340us, used to separate two parts

Each sequence of signals would be th e following format:
```
^>([01]*31)E^>([01]*31)
```
The command repeats again while sending, splitting with `E`.

---

## Command Structure

Each command starts with the start codes `^>`
and follow with 31 bits which is the content of the command.

the meaning of the bits is describe with the chart:

```
0         1         2         3
01234567890123456789012345678901
>0P1MM00Rsxettttff00S111FFFTTTTT

P: Power
M: Mode  [00: Auto; 01: Dehumid; 10: Cooling; 11: Fan]
f: Fuzzy [01: :) ; 10: ⊙w⊙ ; 11: •`_´• ]
F: Fan Speed [000: Auto; 100: Small; 010: Mid; 001: Large]
T: Tempreature [17: 10001 ~ 31: 11111; in reversed bit order]
R: Rhythm
s: Sleeping
S: Sterilization

x: Timer Action [0: Turning Off; 1: Turning On]
e: Timer Enabled
t: Timer Hour [0.5: 1111; 1: 1000 ~ 12: 0011; in reversed bit order]

```

