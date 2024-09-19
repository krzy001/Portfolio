import city.cs.engine.*;

/**
 * A simple character class for every character in the over world levels.
 * Each character requires a shape and image.
 */

public class Character extends Walker {
    public Character(World world, Shape shape, BodyImage image) {
        super(world, shape);
        addImage(image);
    }
}