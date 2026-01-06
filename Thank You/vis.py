import pygame
import numpy as np
from svgpathtools import svg2paths
import cmath
import time

# --- Configuration ---
SVG_FILE = "ty.svg"
WIDTH, HEIGHT = 1400, 800
SAMPLES = 1024  # Higher samples = better detail for multiple paths
TRAIL_LENGTH = 800 
FPS = 60

def get_svg_points(filename, n_samples, target_height=1200):
    paths, _ = svg2paths(filename)
    paths.sort(key=lambda p: p.bbox()[0]) # Left-to-right sorting
    
    pts = []
    connections = [] # Tracks if a point connects to the previous one
    
    for i in range(n_samples):
        t = i / n_samples
        path_idx = int(t * len(paths))
        local_t = (t * len(paths)) % 1
        
        # Determine if this point is a "start" of a new path
        # If it's the very first point or the path_idx just changed
        is_new_path = (i == 0) or (path_idx != int(((i-1)/n_samples) * len(paths)))
        connections.append(not is_new_path)
        
        p = paths[path_idx].point(local_t).conjugate()
        pts.append(p)
    
    pts = np.array(pts)
    center = np.mean(pts)
    pts = (pts - center)
    
    # Scale to fit
    max_dim = max(np.max(np.abs(pts.real)), np.max(np.abs(pts.imag)))
    scale = (target_height / 2) / max_dim
    return pts * scale, connections

def calculate_dft(points):
    N = len(points)
    fourier = []
    # Using centered frequencies for a more balanced "arm" look
    for k in range(N):
        n_val = k - N // 2
        c = np.sum(points * np.exp(-1j * 2 * np.pi * n_val * np.arange(N) / N)) / N
        fourier.append({'freq': n_val, 'amp': abs(c), 'phase': cmath.phase(c)})
    return sorted(fourier, key=lambda x: x['amp'], reverse=True)

def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    clock = pygame.time.Clock()
    
    points, connections = get_svg_points(SVG_FILE, SAMPLES)
    fourier = calculate_dft(points)
    
    path_trace = [] # List of (x, y, connected_to_prev)
    T = 0
    dt = (2 * np.pi) / len(fourier)
    hue = 120 #green

    running = True
    time.sleep(1)
    while running:
        screen.fill((0, 0, 0)) # Absolute Black Background
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        curr_x, curr_y = WIDTH // 2, HEIGHT // 2
        
        # Epicycles
        for f in fourier:
            prev_x, prev_y = curr_x, curr_y
            curr_x += f['amp'] * np.cos(f['freq'] * T + f['phase'])
            curr_y += f['amp'] * np.sin(f['freq'] * T + f['phase'])

            # Draw the radius (vector) from the epicycle center to its endpoint
            pygame.draw.aaline(screen, (120, 120, 150), (int(prev_x), int(prev_y)), (int(curr_x), int(curr_y)))

            # Dimmer epicycles for a "technical" look
            if f['amp'] > 1.5:
                pygame.draw.circle(screen, (40, 40, 50), (int(prev_x), int(prev_y)), int(max(1, f['amp'])), 1)
                pygame.draw.circle(screen, (180, 180, 200), (int(curr_x), int(curr_y)), 2)

        # Sync the path trace with the jump logic
        idx = int((T / (2 * np.pi)) * len(points)) % len(points)
        path_trace.append((curr_x, curr_y, connections[idx]))
        
        if len(path_trace) > TRAIL_LENGTH:
            path_trace.pop(0)

        # Draw the Path
        for i in range(1, len(path_trace)):
            # If the point says it's NOT connected to the previous, skip drawing
            if not path_trace[i][2]: 
                continue 
            
            # Color calculation
            color = pygame.Color(0)
            current_hue = (hue + (i * 0.1)) % 360
            color.hsva = (hue, 80, 100, 100) #replace hue with current_hue for gradient effect
            
            # Fade out older parts of the trail
            fade_factor = (i / len(path_trace)) ** 0.5
            final_color = color.lerp((0, 0, 0), 0) #replace 0 with 1 - fade_factor for fading effect
            
            # Thinner line (width=1)
            pygame.draw.line(screen, final_color, 
                             (path_trace[i-1][0], path_trace[i-1][1]), 
                             (path_trace[i][0], path_trace[i][1]), 4)

        # hue = (hue + 0.5) % 360
        T += dt
        if T > 2 * np.pi:
            T = 0
            
        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()

if __name__ == "__main__":
    main()