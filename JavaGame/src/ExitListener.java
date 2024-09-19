import city.cs.engine.CollisionEvent;
import city.cs.engine.CollisionListener;

/**
 * This class establishes what happens when the player collides with the exit
 */

public class ExitListener implements CollisionListener {
    private Game game;

    public ExitListener(Game game) {
        this.game = game;
    }

    /**
     * This method is executed every time a body collides with the exit of the stage
     * This allows the player to move on to the next stage when allowed to
     *
     * The player must first "complete" the level before moving onto the next level. "game.isCurrentLevelCompleted"
     * usually becomes true after the player has defeated enough enemies in the level.
     * @param e Whenever anything collides with the exit body
     */
    @Override
    public void collide(CollisionEvent e) {
        Player player = game.getPlayer();
        //If it was the player that collided with the exit and the requirements have been met to proceed to the next stage...
        if (e.getOtherBody() == player && game.isCurrentLevelCompleted()) {
            //...call the class to go to the next level.
            game.goNextLevel();
        }
    }
}
