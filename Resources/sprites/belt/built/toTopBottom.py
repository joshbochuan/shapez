from PIL import Image
import os

def split_transparency(prefix="forward_", count=14):
    for i in range(count):
        filename = f"{prefix}{i}.png"
        
        if not os.path.exists(filename):
            print(f"File not found: {filename}")
            continue
        
        img = Image.open(filename).convert("RGBA")
        width, height = img.size
        
        # Create copies
        top_img = img.copy()
        bottom_img = img.copy()
        
        pixels_top = top_img.load()
        pixels_bottom = bottom_img.load()
        
        # Make bottom half transparent (for top image)
        for y in range(height // 2, height):
            for x in range(width):
                r, g, b, a = pixels_top[x, y]
                pixels_top[x, y] = (r, g, b, 0)
        
        # Make top half transparent (for bottom image)
        for y in range(0, height // 2):
            for x in range(width):
                r, g, b, a = pixels_bottom[x, y]
                pixels_bottom[x, y] = (r, g, b, 0)
        
        # Save results
        top_img.save(f"{prefix}{i}_top.png")
        bottom_img.save(f"{prefix}{i}_bottom.png")
        
        print(f"Processed {filename}")

# Run it
split_transparency()