import numpy as np
from PIL import Image

img_file = "./100098.png"
lb = np.array(Image.open(img_file), dtype=np.float32)
print(lb)