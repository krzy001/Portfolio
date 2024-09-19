import city.cs.engine.World;
import org.jbox2d.common.Vec2;

/**
 * The game will be designed in a way for the player to have to traverse through paths and fight
 * their way to the end. The more enemies the player fights, the more powerful they become as
 * defeating enemies grants the player experience points and with enough experience points the
 * player will level up and their health and attack will increase.
 */

public abstract class GameLevel extends World {

    private Player player;
    private Exit exit;

    public Player getPlayer() {
        return player;
    }

    public Exit getExit(){
        return exit;
    }

    /**
    * Every level class will be an extension of this abstract class, and it is set up so every level must contain
    * a starting position for the player, exit position to proceed to the next level, a boolean stating whether or
    * not the requirements for that level have been met, and the level number
     * @param game The main class with access to most of the important methods and variables
     */
    public void populate(Game game){
        player = new Player(this);
        player.setPosition(startPosition());
        player.setEnemyCount(game.getENEMY_KILLED());
        exit = new Exit(this);
        exit.setPosition(exitPosition());
        exit.addCollisionListener(new ExitListener(game));
        game.getFrame().requestFocus();
    }

    public abstract Vec2 startPosition();

    public abstract Vec2 exitPosition();

    public abstract boolean isCompleted();

    public abstract int getLevelNumber();
}
