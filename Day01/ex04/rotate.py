from load_image import ft_load
import numpy as np
import matplotlib.pyplot as plt

def ft_transpose(array: np.ndarray)-> np.ndarray:
    """
    Manually transpose a 2D array .
    Converts array[i][j] to array[j][i].
    """
    if len(array.shape) == 3 and array.shape[2] == 1:
        array = array[:, :, 0]
    
    height = width = 400
    transposed = np.zeros((width, height), dtype=array.dtype)
    
    for i in range(height):
        for j in range(width):
            transposed[j][i] = array[i][j]
    
    return transposed

def main():
    """
    Main function to load, slice, transpose and display an image.
    """
    try:
        image = ft_load("animal.jpeg")
        if image is None:
            return
        
        height = image.shape[0]
        width = image.shape[1]
        
        if height >= 400 and width >= 400:
            x_start = (width - 400) // 2 + 100
            y_start = (height - 400) // 2
        else:
            raise ValueError("Image too small")
        
        zoomed = image[y_start:y_start + 400, x_start:x_start + 400]
        
        if len(zoomed.shape) == 3:
            zoomed_gray = zoomed[:, :, 0:1]
        else:
            zoomed_gray = zoomed
        
        print(f"The shape of image is: {zoomed_gray.shape}")
        print(zoomed_gray)
        
        transposed = ft_transpose(zoomed_gray)
        
        print(f"New shape after Transpose: {transposed.shape}")
        print(transposed)
        
        plt.imshow(transposed, cmap='gray')
        plt.title("Rotated Image")
        plt.xlabel("X axis")
        plt.ylabel("Y axis")
        plt.show()
        
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()
