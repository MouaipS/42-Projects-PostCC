import numpy as np
import matplotlib.pyplot as plt

def ft_invert(array: np.ndarray) -> np.ndarray:
    """
    Inverts the color of the image received.
    Uses only operators: =, +, -, *
    """
    invert = 255 - array
    plt.imshow(invert)
    plt.title("Isnvert")
    plt.axis('off')
    plt.show()
    
    return invert


def ft_red(array: np.ndarray) -> np.ndarray:
    """
    Applies a red filter to the image.
    Only red channel is kept, green and blue are zeroed.
    Uses only operators: =, *
    """
    red_filter = array.copy()
    
    red_filter[:, :, 1] = red_filter[:, :, 1] * 0
    red_filter[:, :, 2] = red_filter[:, :, 2] * 0
    
    plt.imshow(red_filter)
    plt.title("Red Filter")
    plt.axis('off')
    plt.show()
    
    return red_filter

def ft_green(array: np.ndarray) -> np.ndarray:
    """
    Applies a green filter to the image.
    Only green channel is kept, red and blue are zeroed.
    Uses only operators: =, -
    """
    green_filter = array.copy()
    green_filter[:, :, 0] = green_filter[:, :, 0] - green_filter[:, :, 0]
    green_filter[:, :, 2] = green_filter[:, :, 2] - green_filter[:, :, 2]
    
    plt.imshow(green_filter)
    plt.title("Green Filter")
    plt.axis('off')
    plt.show()
    
    return green_filter

def ft_blue(array: np.ndarray) -> np.ndarray:
    """
    Applies a blue filter to the image.
    Only blue channel is kept, red and green are zeroed.
    Uses only operator: =
    """
    blue_filter = array.copy()
    # Mettre red (canal 0) et green (canal 1) à zéro avec seulement =
    blue_filter[:, :, 0] = 0
    blue_filter[:, :, 1] = 0
    
    plt.imshow(blue_filter)
    plt.title("Blue Filter")
    plt.axis('off')
    plt.show()
    
    return blue_filter

def ft_grey(array: np.ndarray) -> np.ndarray:
    """
    Converts the image to grayscale using weighted average.
    Uses only operators: =, /
    """
    grey_filter = array.copy()
    
    # Formule standard: 0.299*R + 0.587*G + 0.114*B
    # Mais on ne peut pas utiliser +, donc on utilise un seul canal
    # Approximation simple : prendre juste un canal (par ex. red)
    
    grey_value = array[:, :, 0] / 1  # Prend juste le canal rouge
    
    # Ou mieux: moyenne en utilisant numpy.mean (fonction, pas opérateur)
    grey_value = np.mean(array, axis=2).astype(np.uint8)
    
    grey_filter[:, :, 0] = grey_value
    grey_filter[:, :, 1] = grey_value
    grey_filter[:, :, 2] = grey_value
    
    plt.imshow(grey_filter, cmap='gray')
    plt.title("Greyscale")
    plt.axis('off')
    plt.show()
    
    return grey_filter
