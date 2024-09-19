import city.cs.engine.*;

/**
 * A simple class creating the box shape and image of the exits of every level.
 */

public class Exit extends StaticBody {

    /**
     *
     * @param world The level which the player has approached
     */
    public Exit(World world) {
        super(world, new BoxShape(0.55f, 1.4f));
        addImage(new BodyImage("data/open_exit.png", 2.8f));
    }
}
