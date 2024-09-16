import numpy as np
import cv2
from skimage.draw import line
import matplotlib.pyplot as plt

def hough_transform(img):
    height, width = img.shape
    max_rho = int(np.sqrt(height**2 + width**2))
    accumulator = np.zeros((2 * max_rho, 180), dtype=np.uint64)
    
    for y in range(height):
        for x in range(width):
            if img[y, x] > 0:
                for theta in range(0, 180):
                    rho = int(x * np.cos(np.deg2rad(theta)) + y * np.sin(np.deg2rad(theta)))
                    accumulator[rho + max_rho, theta] += 1
    
    plt.imshow(accumulator)
    plt.show()

    threshold = 0.9*np.max(accumulator)

    lines = np.where(accumulator > threshold)
    rho_values = lines[0] - max_rho
    theta_values = lines[1]
    
    detected_lines = []
    for rho, theta in zip(rho_values, theta_values):
        a = np.cos(np.deg2rad(theta))
        b = np.sin(np.deg2rad(theta))
        x0 = a * rho
        y0 = b * rho
        x1 = int(x0 + 1000 * (-b))
        y1 = int(y0 + 1000 * (a))
        x2 = int(x0 - 1000 * (-b))
        y2 = int(y0 - 1000 * (a))
        detected_lines.append(((x1, y1), (x2, y2)))
    
    return detected_lines

# 生成随机图像
img = np.zeros((100, 150), dtype=bool)
img[30, :] = 255
img[:, 65] = 255
img[35:45, 35:50] = 255
rr, cc = line(60, 130, 80, 10)
img[rr, cc] = 255
img += np.random.random(img.shape) > 0.95
img = img.astype(np.uint8) * 255  # Convert boolean array to uint8 image

detected_lines = hough_transform(img)
rgb_image = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
for line in detected_lines:
    cv2.line(rgb_image, line[0], line[1], (0, 0, 255), 2)

# plt.imshow(img)
# plt.show()

cv2.imshow('Hough Lines', rgb_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
cv2.imwrite('line.jpg', rgb_image)