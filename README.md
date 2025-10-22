# Stewart Platform - 6-DOF Parallel Manipulator

A mechanical design project for a Stewart Platform (Gough-Stewart platform) featuring 6 degrees of freedom motion using MG90S micro servo actuators.

## Overview

The Stewart Platform is a parallel manipulator with six prismatic actuators, commonly used in flight simulators, vehicle simulators, and precision positioning systems. This design provides full 6-DOF motion capability: translation along X, Y, Z axes and rotation around all three axes (roll, pitch, yaw).

## Project Structure

```
Steward-Platform/
├── cad/
│   ├── parts/          # Individual component parts (.SLDPRT)
│   ├── assemblies/     # Full assembly files (.SLDASM)
│   └── drawings/       # Technical drawings (.SLDDRW)
├── firmware/           # ESP32 Arduino code
├── electronics/        # Circuit board design files (EagleCAD/EasyEDA)
├── docs/               # Documentation and specifications
├── media/
│   ├── images/         # Renders, textures, and decals
│   ├── videos/         # Motion simulation videos
│   └── hdri/           # HDR environment maps for rendering
└── README.md
```

## Key Components

### Platform Structure
- **Top Plate** - Moving platform (upper)
- **Bottom Plate** - Fixed base (lower)
- **Middle Plate** - Intermediate structural support
- **Circuit Board** - Electronics mounting

### Actuation System
- **MG90S Micro Servos** (6x) - 9g hobby servos for actuation
- **Servo Horns** - Attachment points for linkages
- **Rods/Links** - Connecting elements between servos and platform
- **Ball Links** - Universal joints for full range of motion

### Electronics & Control
- **ESP32 Microcontroller** - Controls all 6 servos with 12 DOF motion patterns
- **Custom PCB** - Circuit board design available in `electronics/`
- **Firmware** - Arduino-based control code in `firmware/StewartPlatform.ino`

## Software Requirements

### CAD Software
- **SolidWorks** - Required to open and edit:
  - `.SLDPRT` - Part files
  - `.SLDASM` - Assembly files
  - `.SLDDRW` - Drawing files

### Firmware Development
- **Arduino IDE** with ESP32 board support
- **Required Libraries**:
  - ESP32Servo.h
- **Board Packages**:
  - `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json`
  - `https://espressif.github.io/arduino-esp32/package_esp32_index.json`

### Electronics Design
- **EasyEDA** or compatible PCB design software for viewing/editing circuit board files

## Design Specifications

### Mechanical
- **Actuators**: 6x MG90S micro servos
- **Degrees of Freedom**: 6 (X, Y, Z, Roll, Pitch, Yaw)
- **Architecture**: Parallel manipulator (all actuators work together)
- **Joint Type**: Ball-and-socket/spherical joints
- **Servo Range**: 10° - 170° (configurable in firmware)

### Electronic
- **Controller**: ESP32 microcontroller
- **Servo Control**: 6 independent PWM channels
- **Motion Patterns**: 12 pre-programmed DOF states (Heave, Surge, Sway, Yaw, Pitch, Roll)
- **Calibration**: Individual servo trim adjustment via serial commands
- **Interpolation**: Smooth 500ms transitions between motion states

## Documentation

- `JB_SP_Measurement.xlsx` - Platform measurements and geometry specifications
- `StewartPlatformMotionStudyRubric.xlsx` - Motion study evaluation criteria
- `PrimaryDrawingtemplate.pdf` - Technical drawing template

## Motion Simulation

Motion studies and simulations are performed in SolidWorks Motion Analysis. Example simulation video available in `media/videos/`.

## Development

### Modifying the CAD Design
1. Open `cad/assemblies/JBJBStewardPlatform.SLDASM` in SolidWorks
2. Edit individual parts in the `cad/parts/` directory as needed
3. Verify assembly constraints maintain proper DOF
4. Run motion studies to validate range of motion
5. Update technical drawings if dimensions change
6. Export new servo positions to update firmware motion arrays

### Uploading Firmware
1. Install Arduino IDE with ESP32 board support (see Software Requirements)
2. Install required ESP32Servo library
3. Open `firmware/StewartPlatform.ino`
4. Connect ESP32 via USB
5. Select correct board and COM port
6. Upload to ESP32

### Serial Commands
The firmware supports serial commands for calibration:
- `C` - Get/set calibration mode (C=0 or C=1)
- `T` - Get/set trim for all servos (e.g., T=5)
- `T0` through `T5` - Get/set individual servo trim (e.g., T0=-3)
- `O` - Get/set platform offset (e.g., O=10)

### Best Practices
- Maintain proper assembly constraints for 6-DOF motion
- Verify no singularities in the workspace
- Keep ball joint positions within servo range
- Test full range of motion in simulation before fabrication
- Calibrate servo trim values before running motion sequences
- Use serial monitor at 115200 baud for debugging

## License

See repository license file for details.

## Contributing

Contributions, issues, and feature requests are welcome!
