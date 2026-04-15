# Design Document: Smooth Tile Movement

## Overview

This design implements Pokemon-style smooth tile movement by replacing the current fixed-pixel-speed approach with frame-based linear interpolation. Each tile movement will complete in a fixed number of frames (configurable, default 8 frames), providing consistent and smooth movement regardless of frame rate variations.

## Architecture

The movement system will transition from:
- **Current**: Speed-based (move N pixels per frame until distance < threshold)
- **New**: Frame-based (move over exactly N frames using linear interpolation)

### Key Changes

1. Add frame counter to track movement progress
2. Replace pixel speed with frame duration
3. Use linear interpolation: `position = start + (target - start) * (frame / duration)`
4. Update camera every frame during movement (not just at end)

## Components and Interfaces

### Modified Structure: `SB` (Hero)

**Location**: `game.h`

**Current Fields**:
```c
float speeding;  // Pixels per frame
bool moving;
```

**New Fields**:
```c
int move_frames;        // Total frames for one tile movement
int move_frame_count;   // Current frame in movement (0 to move_frames)
float start_x, start_y; // Starting position for interpolation
bool moving;
```

**Removed Fields**:
- `speeding` (replaced by frame-based system)

### Modified Function: `player_update(Game *game)`

**Location**: `input.c`

**New Algorithm**:

```
1. Get keyboard input and update facing direction

2. Movement Initiation (if !moving and direction pressed):
   a. Check target tile walkability
   b. If walkable:
      - Set moving = true
      - Store start_x = hero.x, start_y = hero.y
      - Update tile_x, tile_y to target
      - Calculate px, py (target pixel position)
      - Reset move_frame_count = 0

3. Movement Interpolation (if moving):
   a. Increment move_frame_count
   b. Calculate progress = move_frame_count / move_frames
   c. Interpolate: hero.x = start_x + (px - start_x) * progress
   d. Interpolate: hero.y = start_y + (py - start_y) * progress
   e. Update camera position based on current hero.x, hero.y
   f. Update hero_screen position for rendering
   
   g. If move_frame_count >= move_frames:
      - Snap to exact target: hero.x = px, hero.y = py
      - Check for exit tiles
      - Set moving = false
```

### Configuration Constants

**Location**: `game.h`

```c
#define MOVE_FRAMES 8  // Frames per tile movement (Pokemon uses ~8-12)
```

Adjusting this value changes movement speed:
- Lower values (6-7) = faster, snappier movement
- Higher values (10-12) = slower, more deliberate movement
- Pokemon typically uses 8 frames at 60 FPS

## Data Models

### Hero Structure Changes

```c
typedef struct {
    int tile_x, tile_y;           // Current tile position
    SDL_Texture *hero;
    int px, py;                   // Target pixel position
    float x, y;                   // Current interpolated position
    float start_x, start_y;       // NEW: Starting position for interpolation
    int w, h;
    SDL_Rect hero_screen;
    int facing;
    int move_frames;              // NEW: Total frames per tile
    int move_frame_count;         // NEW: Current frame counter
    bool moving;
} SB;
```

### Initialization Changes

**Location**: `game.c` in `Game_Init()`

```c
game->hero.move_frames = MOVE_FRAMES;
game->hero.move_frame_count = 0;
game->hero.start_x = game->hero.x;
game->hero.start_y = game->hero.y;
// Remove: game->hero.speeding = 2.0f;
```

## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system—essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees.*

### Property 1: Fixed Duration Movement
*For any* tile movement, the movement should complete in exactly `move_frames` frames, regardless of tile distance or direction.

**Validates: Requirements 1.1, 1.2, 1.3**

### Property 2: Linear Interpolation Progress
*For any* frame during movement, the interpolation progress should equal `move_frame_count / move_frames` and be in the range [0.0, 1.0].

**Validates: Requirements 2.1, 2.2**

### Property 3: Position Bounds
*For any* frame during movement from start to target, the current position should be between start and target coordinates (inclusive).

**Validates: Requirements 2.3**

### Property 4: Exact Target Arrival
*For any* completed movement, the final position should exactly equal the target pixel position (no floating point drift).

**Validates: Requirements 2.4**

### Property 5: Camera Continuity
*For any* frame during movement, the camera position should be updated based on the hero's current interpolated position.

**Validates: Requirements 3.1, 3.2, 3.3, 3.4**

## Error Handling

No new error conditions are introduced. Existing error handling remains:
- Boundary checks for tile coordinates
- Camera boundary clamping
- Map transition validation

### Edge Cases

1. **Frame count overflow**: Clamped by checking `>= move_frames` instead of `==`
2. **Floating point precision**: Final position explicitly snapped to target
3. **Zero-frame movement**: Prevented by using reasonable MOVE_FRAMES constant (minimum 4)

## Testing Strategy

### Manual Testing

Since this involves real-time rendering and user input:

1. **Movement Smoothness**: Verify hero glides smoothly between tiles
2. **Consistent Speed**: Verify all movements take the same time
3. **Camera Following**: Verify camera follows smoothly without jumps
4. **Direction Changes**: Verify facing updates immediately but movement completes before new movement starts
5. **Wall Collision**: Verify blocked movements don't start interpolation
6. **Speed Tuning**: Test different MOVE_FRAMES values (6, 8, 10, 12) to find optimal feel

### Expected Results

- **8 frames at 60 FPS** = ~133ms per tile (Pokemon-like speed)
- **Smooth interpolation** with no visible stuttering
- **Camera follows continuously** during movement
- **No position drift** or accumulation errors

### Performance Considerations

- Linear interpolation is computationally cheap (2 multiplications, 2 additions per frame)
- Camera updates every frame add minimal overhead
- No performance degradation expected compared to current implementation
