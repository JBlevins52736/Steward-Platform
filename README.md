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

## Software Requirements

- **SolidWorks** - CAD software required to open and edit:
  - `.SLDPRT` - Part files
  - `.SLDASM` - Assembly files
  - `.SLDDRW` - Drawing files

## Design Specifications

- **Actuators**: 6x MG90S micro servos
- **Degrees of Freedom**: 6 (X, Y, Z, Roll, Pitch, Yaw)
- **Architecture**: Parallel manipulator (all actuators work together)
- **Joint Type**: Ball-and-socket/spherical joints

## Documentation

- `JB_SP_Measurement.xlsx` - Platform measurements and geometry specifications
- `StewartPlatformMotionStudyRubric.xlsx` - Motion study evaluation criteria
- `PrimaryDrawingtemplate.pdf` - Technical drawing template

## Motion Simulation

Motion studies and simulations are performed in SolidWorks Motion Analysis. Example simulation video available in `media/videos/`.

## Development

### Modifying the Design
1. Open `cad/assemblies/JBJBStewardPlatform.SLDASM` in SolidWorks
2. Edit individual parts in the `cad/parts/` directory as needed
3. Verify assembly constraints maintain proper DOF
4. Run motion studies to validate range of motion
5. Update technical drawings if dimensions change

### Best Practices
- Maintain proper assembly constraints for 6-DOF motion
- Verify no singularities in the workspace
- Keep ball joint positions within servo range
- Test full range of motion in simulation before fabrication

## License

See repository license file for details.

## Contributing

Contributions, issues, and feature requests are welcome!
