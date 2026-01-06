import pygame
import numpy as np
from scipy.integrate import odeint
import time

# Initialize Pygame and set fullscreen display
pygame.init()
screen = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
pygame.display.set_caption("Double Pendulum on Moving Carriage (AcroCart)")
WIDTH, HEIGHT = screen.get_size()
SCALE = int(min(WIDTH, HEIGHT) / 4)
CENTER = (WIDTH // 2, HEIGHT // 2)

# Physical parameters
g = 9.81  # gravity
L1, L2 = 1.0, 1.0  # lengths of pendulum arms
m1, m2 = 1.0, 1.0  # masses of pendulums
mc = 2.0  # mass of carriage
omega_drive = 2.0  # frequency of carriage oscillation
A = 1.0  # amplitude of carriage oscillation
b = 0.0  # damping coefficient (friction)

# Colors (user-specified)
BG_COLOR = (20, 22, 30)
RAIL_COLOR = (60, 70, 90)
CARRIAGE_COLOR = (180, 180, 200)
PENDULUM_COLOR = (120, 200, 255)
JOINT_COLOR = (255, 120, 120)

# Initial conditions (slightly off vertical)
theta1_0 = 0  # first pendulum angle
theta2_0 = 0  # second pendulum angle
omega1_0 = 0.0  # first pendulum angular velocity
omega2_0 = 0.0  # second pendulum angular velocity

def get_carriage_motion(t):
    # Position and acceleration of the carriage
    x = A * np.sin(omega_drive * t)
    x_dot = A * omega_drive * np.cos(omega_drive * t)
    x_ddot = -A * omega_drive**2 * np.sin(omega_drive * t)
    return x, x_dot, x_ddot

def derivatives(state, t):
    theta1, omega1, theta2, omega2 = state
    
    # Get carriage motion
    x, x_dot, x_ddot = get_carriage_motion(t)
    
    # Equations of motion for double pendulum on moving carriage
    c = np.cos(theta1 - theta2)
    s = np.sin(theta1 - theta2)
    
    # Additional terms due to carriage motion
    horiz_acc = x_ddot
    
    theta1_dot = omega1
    theta2_dot = omega2
    
    # Modified equations including horizontal acceleration and damping
    omega1_dot = (-g*(2*m1 + m2)*np.sin(theta1) - m2*g*np.sin(theta1 - 2*theta2)
                 - 2*s*m2*(omega2**2*L2 + omega1**2*L1*c)
                 - (2*m1 + m2)*horiz_acc*np.cos(theta1)/L1  # Fixed sign for correct inertial effect
                 - b*omega1) / (L1*(2*m1 + m2 - m2*c**2))
    
    omega2_dot = (2*s*(omega1**2*L1*(m1 + m2) + g*(m1 + m2)*np.cos(theta1)
                 + omega2**2*L2*m2*c)
                 - 2*m2*horiz_acc*np.cos(theta2)/L2  # Fixed sign for correct inertial effect
                 - b*omega2) / (L2*(2*m1 + m2 - m2*c**2))
    
    return np.array([theta1_dot, omega1_dot, theta2_dot, omega2_dot])

# Clock (display already initialized above in fullscreen)
clock = pygame.time.Clock()

# Initial state
state = np.array([theta1_0, omega1_0, theta2_0, omega2_0])

running = True
last_time = time.time()
start_time = time.time()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False
    
    # Calculate time step
    current_time = time.time()
    dt = current_time - last_time
    last_time = current_time
    
    # Get current simulation time
    t = current_time - start_time
    
    # Update physics
    t_span = np.array([t, t + dt])
    state = odeint(derivatives, state, t_span)[-1]  # Get final state
    theta1, omega1, theta2, omega2 = state
    
    # Get carriage position
    carriage_x, _, _ = get_carriage_motion(t)
    
    # Calculate positions (relative to carriage)
    x1 = L1 * np.sin(theta1)
    y1 = -L1 * np.cos(theta1)
    x2 = x1 + L2 * np.sin(theta2)
    y2 = y1 - L2 * np.cos(theta2)
    
    # Add carriage position to x coordinates
    carriage_screen_x = int(CENTER[0] + carriage_x * SCALE)
    screen_x1 = int(carriage_screen_x + x1 * SCALE)
    screen_y1 = int(CENTER[1] - y1 * SCALE)
    screen_x2 = int(carriage_screen_x + x2 * SCALE)
    screen_y2 = int(CENTER[1] - y2 * SCALE)
    
    # Draw
    screen.fill(BG_COLOR)

    # Draw rail behind the carriage
    rail_height = 8
    pygame.draw.rect(screen, RAIL_COLOR, (0, CENTER[1] - rail_height // 2, WIDTH, rail_height))

    # Draw carriage
    carriage_width = 60
    carriage_height = 20
    pygame.draw.rect(screen, CARRIAGE_COLOR,
                     (carriage_screen_x - carriage_width//2,
                      CENTER[1] - carriage_height//2,
                      carriage_width,
                      carriage_height))

    # Draw pendulum arms
    pygame.draw.line(screen, PENDULUM_COLOR, (carriage_screen_x, CENTER[1]), (screen_x1, screen_y1), 3)
    pygame.draw.line(screen, PENDULUM_COLOR, (screen_x1, screen_y1), (screen_x2, screen_y2), 3)

    # Draw joints/masses
    pygame.draw.circle(screen, JOINT_COLOR, (screen_x1, screen_y1), 10)
    pygame.draw.circle(screen, JOINT_COLOR, (screen_x2, screen_y2), 10)
    
    pygame.display.flip()
    clock.tick(90)  # Limit to 60 FPS

pygame.quit()