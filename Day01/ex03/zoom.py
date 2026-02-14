from load_image import ft_load
import numpy as np
import matplotlib.pyplot as plt

def zoom_image(image: np.ndarray, x_start: int, y_start: int) -> np.ndarray:
    """
    Slice a 400x400 area from the image starting at (x_start, y_start).
    """
    return image[y_start:y_start + 400, x_start:x_start + 400]

def main():
    """
    	Main function to load, zoom, and display an image.
    """
    
    try:
        image = ft_load("animal.jpeg")
        if image is None:
            return
        if len(image.shape) == 3:
            print(f"Number of channels: {image.shape[2]}")
        else:
            print("Number of channels: 1")
        
        height = image.shape[0]
        width = image.shape[1]
        if height >= 400 and width >= 400:
            x_start = (width - 400) // 2
            y_start = (height - 400) //2
        else:
            raise ValueError("Image too small")
        zoomed = zoom_image(image, x_start, y_start)
        
        
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()