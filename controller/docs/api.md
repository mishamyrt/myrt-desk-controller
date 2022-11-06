# UDP Packet structure

This describes the UDP API of the desk, which was designed to replace the JSON REST API. The overhead proved to be too much for ESP32.

## Formats

### Input
```sh
<lenght> <domain> <command> [<arg_1>, <arg_2>, <arg_3>, ..., <arg_(lenght - 2)>]
```

### Output

```sh
<lenght> <domain> <command> <status_code>
```

If the command executed successfully, the `status_code` will be 0.


The format may be different for commands that describe a state.

## Domains

### `0x0` Backlight

The domain for managing everything related to backlighting.

* `0x0` - Read current backlight state.
  ```sh
  # Input:
  2 0 0

  # Output:
  8 0 <enabled> <effect> <r> <g> <b> <temperature> <brightness>
  ```
* `0x1` - Set effect
  ```sh
  # Input:
  3 0 1 <effect_id>
  ```
* `0x2` - Set color
  ```sh
  # Input:
  5 0 2 <r> <g> <b>
  ```
* `0x3` - Set white temperature
  ```sh
  # Input:
  3 0 3 <temperature>
  ```

### `0x1` Legs

The domain for managing desk legs height.
* `0x0` - Read legs height.
  ```sh
  # Input:
  2 1 0

  # Output:
  2 1 <height>
  ```
* `0x1` - Set height.
  ```sh
  # Input:
  3 1 1 <height>
  ```
