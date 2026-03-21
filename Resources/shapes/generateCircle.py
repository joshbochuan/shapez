from PIL import Image, ImageDraw

def recover_alpha_and_color(C1, C2, A, B):
    """
    C1, C2: observed colors (R,G,B) on backgrounds A and B
    A, B: background colors (R,G,B)

    Returns:
        alpha (float), original_color (R,G,B)
    """

    alphas = []

    # Solve alpha per channel
    for i in range(3):
        c1 = C1[i]
        c2 = C2[i]
        a = A[i]
        b = B[i]

        if a != b:
            alpha = 1 - (c1 - c2) / (a - b)
            alphas.append(alpha)

    if not alphas:
        raise ValueError("Cannot solve alpha: backgrounds are identical in all channels.")

    # Average alpha from valid channels
    alpha = sum(alphas) / len(alphas)

    # Clamp alpha to valid range
    alpha = max(0.0, min(1.0, alpha))

    # Recover original color
    original = []
    for i in range(3):
        c1 = C1[i]
        a = A[i]

        if alpha == 0:
            value = 0
        else:
            value = (c1 - (1 - alpha) * a) / alpha

        value = max(0, min(255, round(value)))
        original.append(value)

    return alpha, tuple(original)
    

def create_image(alpha, original_rgb):
    # Convert alpha to 0–255
    alpha_255 = int(round(alpha * 255))

    # Create transparent image
    size = 512
    image = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)

    # Circle parameters
    center = size // 2
    radius = 210
    bbox = [
        center - radius,
        center - radius,
        center + radius,
        center + radius
    ]

    # Draw circle
    draw.ellipse(bbox, fill=(*original_rgb, alpha_255))

    # Save image
    image.save("shape.png")
    print("Saved as shape.png")


if __name__ == "__main__":
    # Example inputs
    C1 = (192, 195, 201)   # belt background
    A  = (210, 212, 217)   # shade on belt

    C2 = (176, 179, 185)   # on belt arrow
    B  = (192, 194, 199)   # shade on belt arrow

    alpha, original = recover_alpha_and_color(C1, C2, A, B)

    print("Recovered alpha:", alpha)
    print("Recovered original color:", original)
    create_image(alpha, original)