from PIL import Image
import numpy as np


def ft_load(path: str)-> np.ndarray:
    """
    Load an image from the given path and print its format and pixel content.
    path: Path to the image file

        Returns:
        numpy array containing the image pixel data in RGB format
    Raises:
        FileNotFoundError: If the image file doesn't exist
        Exception: If there's an error loading the image
    """
    try:
        image = Image.open(path)
        image_array = np.array(image)
        print(f"The Shape of image is: {image_array.shape}")
        return image_array
    except FileNotFoundError:
        print(f"Error: File '{path}' not found.")
        return
    except PermissionError:
        print(f"Error: Permission denied to read '{path}'.")
        return