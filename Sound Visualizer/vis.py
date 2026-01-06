import numpy as np
import soundfile as sf
import pygame
from collections import deque
import time


# User Controls

coherence = 0

sample_rate = 44100
frame_rate = 60
samples_per_frame = sample_rate // frame_rate

lowpass_alpha = 0.12
highpass_alpha = 0.20
smooth_x = 0.20
smooth_y = 0.25

delay_ms = 10
delay_samples = int(sample_rate * delay_ms / 1000)

gain = 350


# Load Audio File

audio_data, file_sr = sf.read("maker.mp3")

# Resample if needed
if file_sr != sample_rate:
    import librosa
    audio_data = librosa.resample(audio_data.T, orig_sr=file_sr, target_sr=sample_rate).T

# Ensure stereo
if len(audio_data.shape) == 1:
    audio_data = np.stack((audio_data, audio_data), axis=1)

num_samples = len(audio_data)
cursor = 0


# PyGame Init

pygame.init()
screen = pygame.display.set_mode((800, 800))
center = np.array([400, 400])

trail_surface = pygame.Surface((800, 800))
trail_surface.set_alpha(40)
trail_surface.fill((0, 0, 0))
time.sleep(2)
clock = pygame.time.Clock()

# Signal State
delay_buf = deque([0.0]*delay_samples, maxlen=delay_samples)

lp_state = 0.0
hp_state = 0.0
x_prev = 0.0
y_prev = 0.0


# Main Loop

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            quit()

    # Loop audio
    if cursor + samples_per_frame >= num_samples:
        cursor = 0

    frame = audio_data[cursor:cursor + samples_per_frame]
    cursor += samples_per_frame

    L = frame[:, 0]
    R = frame[:, 1]

    # Soft compression
    L = np.tanh(L * 3)
    R = np.tanh(R * 3)

    
    # Filtering
    
    lp = []
    for s in L:
        lp_state = (1 - lowpass_alpha) * lp_state + lowpass_alpha * s
        lp.append(lp_state)
    Xsig = np.array(lp)

    hp = []
    for s in R:
        hp_state = (1 - highpass_alpha) * hp_state + highpass_alpha * s
        hp.append(s - hp_state)
    hp = np.array(hp)

    delayed = []
    for v in hp:
        delayed.append(delay_buf[0])
        delay_buf.append(v)
    Ysig = np.array(delayed)

    
    # COHERENCE CONTROL
    
    Ysig = coherence * Xsig + (1 - coherence) * Ysig
    Ysig *= (1 - 0.5 * coherence)

    pts = []

    for x, y in zip(Xsig, Ysig):
        x = (1 - smooth_x)*x_prev + smooth_x*x
        y = (1 - smooth_y)*y_prev + smooth_y*y
        x_prev, y_prev = x, y

        pts.append(center + gain*np.array([x, y]))

    
    # DRAW
    
    screen.blit(trail_surface, (0, 0))
    trail_surface.fill((0, 0, 0))

    if len(pts) > 1:
        pygame.draw.aalines(screen, (0, 255, 0), False, pts)

    pygame.display.flip()
    clock.tick(frame_rate)