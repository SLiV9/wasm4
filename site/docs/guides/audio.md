# Playing Audio

import MultiLanguageCode from '@site/src/components/MultiLanguageCode';
import PlayButton from '@site/src/components/PlayButton';

WASM-4's sound system has 4 independent channels. Each channel is dedicated to a different type of
audio waveform.

- 2 [pulse](https://en.wikipedia.org/wiki/Pulse_wave) wave (square wave) channels. The classic chiptune sound.
- 1 [triangle](https://en.wikipedia.org/wiki/Triangle_wave) wave channel. A softer sound, great for bass.
- 1 [noise](https://en.wikipedia.org/wiki/White_noise) channel. A harsh sound, for percussion and effects.

:::info
WASM-4's sound system is closely inspired by the architecture of the Nintendo NES and Gameboy.
:::

## Playing a Tone

The `tone()` function is used to play a tone with a given frequency on a given channel.

`tone (frequency, duration, volume, flags)`

- Frequency is the "pitch", measured in hertz.
- Durations are measured in frames, 1/60ths of a second.
- Volume ranges from 0 (silent) to 100 (full volume).
- Flags sets the channel (0-3), duty cycle (0-3) and panning (0-2).

For example, to play a one second (60 frames) tone of 262 Hz ([middle C](https://pages.mtu.edu/~suits/notefreqs.html)) on the first pulse wave channel:

<MultiLanguageCode>

```typescript
w4.tone(262, 60, 100, w4.TONE_PULSE1);
```

```c
tone(262, 60, 100, TONE_PULSE1);
```

```d
w4.tone(262, 60, 100, w4.tonePulse1);
```

```go
w4.Tone(262, 60, 100, w4.TONE_PULSE1)
```

```lua
tone(262, 60, 100, TONE_PULSE1)
```

```nim
tone(262, 60, 100, TONE_PULSE1)
```

```odin
w4.tone(262, 60, 100, .Pulse1)
```

```porth
import proc tone int int int int in end

$TONE_PULSE1 60 60 262 tone
```

```roland
tone(262, 60, 100, TONE_PULSE1);
```

```rust
tone(262, 60, 100, TONE_PULSE1);
```

```wasm
(import "env" "tone" (func $tone (param i32 i32 i32 i32)))

(call $tone (i32.const 262) (i32.const 60) (i32.const 100) (global.get $TONE_PULSE1))
```

```zig
w4.tone(262, 60, 100, w4.TONE_PULSE1);
```

</MultiLanguageCode>

## Duty Cycle

The [duty cycle](https://en.wikipedia.org/wiki/Duty_cycle) of the pulse channels can be controlled
with additional flags:

| Flag         | Duty Cycle      |
| ---          | ---             |
| `TONE_MODE1` | 12.5% (default) |
| `TONE_MODE2` | 25%             |
| `TONE_MODE3` | 50%             |
| `TONE_MODE4` | 75%             |

For example, to play at 50% duty cycle (square wave):

<MultiLanguageCode>

```typescript
w4.tone(262, 60, 100, w4.TONE_PULSE1 | w4.TONE_MODE3);
```

```c
tone(262, 60, 100, TONE_PULSE1 | TONE_MODE3);
```

```d
w4.tone(262, 60, 100, w4.tonePulse1 | w4.toneMode3);
```

```go
w4.Tone(262, 60, 100, w4.TONE_PULSE1 | w4.TONE_MODE3)
```

```lua
tone(262, 60, 100, TONE_PULSE1 | TONE_MODE3)
```

```nim
tone(262, 60, 100, TONE_PULSE1 or TONE_MODE3)
```

```odin
w4.tone(262, 60, 100, .Pulse1, .Half)
```

```porth
$TONE_PULSE1 $TONE_MODE3 or 100 60 262 tone
```

```roland
tone(262, 60, 100, TONE_PULSE1 | TONE_MODE3);
```

```rust
tone(262, 60, 100, TONE_PULSE1 | TONE_MODE3);
```

```wasm
(call $tone
  (i32.const 262)
  (i32.const 60)
  (i32.const 100)
  (i32.or
    (global.get $TONE_PULSE1)
    (global.get $TONE_MODE3)))
```

```zig
w4.tone(262, 60, 100, w4.TONE_PULSE1 | w4.TONE_MODE3);
```

</MultiLanguageCode>

## Frequency Slide

We can actually pass two different frequencies to `tone()`. The high 16 bits of the `frequency`
parameter is used for a second frequency. If non-zero, it specifies the frequency to slide to over
the duration of the tone.

For example, to slide the tone starting from 262 Hz and up to 523 Hz:

<MultiLanguageCode>

```typescript
w4.tone(262 | (523 << 16), 60, 100, w4.TONE_PULSE1);
```

```c
tone(262 | (523 << 16), 60, 100, TONE_PULSE1);
```

```d
w4.tone(262 | (523 << 16), 60, 100, w4.tonePulse1);
```

```go
w4.Tone(262 | (523 << 16), 60, 100, w4.TONE_PULSE1)
```

```lua
tone(262 | (523 << 16), 60, 100, TONE_PULSE1)
```

```nim
tone(262 or (523 shl 16), 60, 100, TONE_PULSE1)
```

```odin
w4.tone(262 | (523 << 16), 60, 100, .Pulse1)
```

```porth
$TONE_PULSE1 100 60 523 16 shl 262 or tone
```

```roland
tone(262 | (523 << 16), 60, 100, TONE_PULSE1);
```

```rust
tone(262 | (523 << 16), 60, 100, TONE_PULSE1);
```

```wasm
(call $tone
  (i32.or
    (i32.const 262)
    (i32.shl (i32.const 523) (i32.const 16)))
  (i32.const 60)
  (i32.const 100)
  (global.get $TONE_PULSE1))
```

```zig
w4.tone(262 | (523 << 16), 60, 100, w4.TONE_PULSE1);
```

</MultiLanguageCode>


## Volume

There are two volume levels that can be passed to the tone function:

| Volume  | Shift | Description                                                 |
| ---     | ---   | ---                                                         |
| Sustain | 0     | The main volume used for the `sustain` duration.            |
| Peak    | 8     | Optional, the peak volume reached by the `attack` duration. |

If the peak volume is not set (or is set to 0), it defaults to 100.

## ADSR Envelope

[ADSR](https://en.wikipedia.org/wiki/ADSR_envelope) describes how the volume changes over time, and
has 4 time components:

| Time        | Shift | Description                                                             |
| ---         | ---   | ---                                                                     |
| **A**ttack  | 24    | The time it takes to initially ramp up from 0 volume to `peak` volume.  |
| **D**ecay   | 16    | The time taken to ramp down from `peak` volume to the `sustain` volume. |
| **S**ustain | 0     | The time to hold the tone steady at the `sustain` volume.               |
| **R**elease | 8     | The time to ramp back down to 0 volume.                                 |

These times are all measured in frames (1/60th of a second), and can be packed
into the `duration` parameter.

For example, to play a tone that sustains for one second and releases over half a second (30 frames):

<MultiLanguageCode>

```typescript
w4.tone(262, 60 | (30 << 8), 100, w4.TONE_PULSE1);
```

```c
tone(262, 60 | (30 << 8), 100, TONE_PULSE1);
```

```d
w4.tone(262, 60 | (30 << 8), 100, w4.tonePulse1);
```

```go
w4.Tone(262, 60 | (30 << 8), 100, w4.TONE_PULSE1)
```

```lua
tone(262, 60 | (30 << 8), 100, TONE_PULSE1)
```

```odin
w4.tone(262, 60 | (30 << 8), 100, .Pulse1)
```

```porth
$TONE_PULSE1 100 30 8 shl 60 or 262 tone
```

```roland
tone(262, 60 | (30 << 8), 100, TONE_PULSE1);
```

```rust
tone(262, 60 | (30 << 8), 100, TONE_PULSE1);
```

```wasm
(call $tone
  (i32.const 262)
  (i32.or
    (i32.const 60)
    (i32.shl
      (i32.const 30)
      (i32.const 8)))
  (i32.const 100)
  (global.get $TONE_PULSE1))
```

```zig
w4.tone(262, 60 | (30 << 8), 100, w4.TONE_PULSE1);
```

</MultiLanguageCode>

## Panning

Tones can be panned either center (default), far left (`TONE_PAN_LEFT`),
or far right (`TONE_PAN_RIGHT`), similar to on a gameboy.

<MultiLanguageCode>

```typescript
w4.tone(262, 60, 100, w4.TONE_PULSE1 | w4.TONE_PAN_LEFT);
```

```c
tone(262, 60, 100, TONE_PULSE1 | TONE_PAN_LEFT);
```

```d
w4.tone(262, 60, 100, w4.tonePulse1 | w4.tonePanLeft);
```

```go
w4.Tone(262, 60, 100, w4.TONE_PULSE1 | w4.TONE_PAN_LEFT)
```

```lua
tone(262, 60, 100, TONE_PULSE1 | TONE_PAN_LEFT)
```

```nim
tone(262, 60, 100, TONE_PULSE1 or TONE_PAN_LEFT)
```

```odin
w4.tone(262, 60, 100, .Pulse1, .Half, .Left)
```

```porth
$TONE_PAN_LEFT $TONE_PULSE1 or 100 60 262 tone
```

```roland
tone(262, 60, 100, TONE_PULSE1 | TONE_PAN_LEFT);
```

```rust
tone(262, 60, 100, TONE_PULSE1 | TONE_PAN_LEFT);
```

```wasm
(call $tone
  (i32.const 262)
  (i32.const 60)
  (i32.const 100)
  (i32.or
    (global.get $TONE_PULSE1)
    (global.get $TONE_PAN_LEFT)))
```

```zig
w4.tone(262, 60, 100, w4.TONE_PULSE1 | w4.TONE_PAN_LEFT);
```

</MultiLanguageCode>

## Calculating Flags

Setting ADSR flags require the use of various bitwise and bitshift operations. This can be a little confusing to understand.

These functions can be used to understand how they are calculated:

<MultiLanguageCode>

```typescript
function toneFrequency(freq1: i32 = 0, freq2: i32 = 0): u32 {
	return freq1 | (freq2 << 16);
}

function toneDuration(attack: i32 = 0, decay: i32 = 0, sustain: i32 = 0, release: i32 = 0): u32 {
	return (attack << 24) | (decay << 16) | sustain | (release << 8);
}

function toneVolume(peak: i32 = 0, volume: i32 = 0): u32 {
	return (peak << 8) | volume;
}

function toneFlags(channel: i32 = 0, mode: i32 = 0, pan: i32 = 0): u32 {
	return channel | (mode << 2) | (pan << 4);
}
```

```rust
fn tone_frequency(freq1: u32, freq2: u32) -> u32 {
	freq1 | (freq2 << 16)
}

fn tone_duration(attack: u32, decay: u32, sustain: u32, release: u32) -> u32 {
	(attack << 24) | (decay << 16) | sustain | (release << 8)
}

fn tone_volume(peak: u32, volume: u32) -> u32 {
	(peak << 8) | volume
}

fn tone_flags(channel: u32, mode: u32, pan: u32) -> u32 {
	channel | (mode << 2) | (pan << 4)
}
```

</MultiLanguageCode>

## Sound Tool

The sound demo and sound test are a great way to quickly experiment with different sounds and
find values to plug into your game:

<div className="row margin-bottom--lg">
    <PlayButton slug="sound-demo" title="Sound Demo" author="Bruno Garcia" github="aduros" colWidth="3"/>
    <PlayButton slug="sound-test" title="Sound Test" author="Mr.Rafael" github="MrRafael-dev" colWidth="3"/>
</div>
