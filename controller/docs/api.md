# UDP Packet structure

This describes the UDP API of the desk, which was designed to replace the JSON REST API. The overhead proved to be too much for ESP32.

## Formats

### Input
```sh
<length> <domain> <command> [<arg_1>, <arg_2>, <arg_3>, ..., <arg_(args_length)>]
```

### Output

```sh
<length> <domain> <command> <status_code>
```

If the command executed successfully, the `status_code` will be 0.


The format may be different for commands that describe a state.

## Domains

### `0x0` System

System domain.

* `0x0` - Describe.
  ```sh
  # Input:
  2 0 0

  # Output:
  8 0 <wifi_level> <macseg_1> <macseg_2> <macseg_3> <macseg_4> <macseg_5> <macseg_6>
  ```
* `0x1` - Reboot.
  ```sh
  # Input:
  2 0 1
  ```

### `0x1` Backlight

The domain for managing everything related to backlighting.

* `0x0` - Read current backlight state.
  ```sh
  # Input:
  2 1 0

  # Output:
  8 1 <enabled> <effect> <r> <g> <b> <temperature> <brightness>
  ```
* `0x1` - Set effect.
  ```sh
  # Input:
  <args_count + 3> 1 1 <effect_id> [...args]
  ```
* `0x2` - Set effect data.
  ```sh
  # Input:
  <args_count + 2> 1 2 [...args]
  ```
* `0x3` - Set color.
  ```sh
  # Input:
  5 1 3 <r> <g> <b>
  ```
* `0x4` - Set white temperature.
  ```sh
  # Input:
  3 1 4 <temperature>
  ```

### `0x2` Legs

The domain for managing desk legs height.
* `0x0` - Read legs height.
  ```sh
  # Input:
  2 2 0

  # Output:
  2 2 <height>
  ```
* `0x1` - Set height.
  ```sh
  # Input:
  3 2 1 <height>
  ```
