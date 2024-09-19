import city.cs.engine.Walker;
import org.jbox2d.common.Vec2;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

/**
 *The controls for the player in the over world levels. They are only capable of moving around the level
 */

public class ControllerOverWorld extends KeyAdapter {
    //Establishing their walking speed
    private float WALKING_SPEED = 8;

    private Walker body;

    public ControllerOverWorld(Walker body){
        this.body = body;
    }

    /**
     * This method is executed every time a key is pressed on the user's keyboard in the over world levels.
     * This allows the player to traverse across the stages to reach their enemies and the exit.
     *
     * The physics engine doesn't have an option for players to "walk up" or "walk down", only left or right
     * so to work around this the jump function has been used for up and down. It'll work exactly the same way
     * as walking left and right because the gravity of the over world levels are all set to 0, meaning there is
     * no gravity.
     * @param e The event of a key being pressed
     */
    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();

        //If the up arrow key is pressed...
        if (key == KeyEvent.VK_UP) {
            //...The player moves up.
            body.jump(WALKING_SPEED);
        }
        //If the down arrow key is pressed...
        else if (key == KeyEvent.VK_DOWN) {
            //...the player moves down.
            body.jump(-WALKING_SPEED);
        }
        //If the right arrow key is pressed...
        else if (key == KeyEvent.VK_RIGHT) {
            //...the player moves right.
            body.startWalking(WALKING_SPEED);
        }
        //If the left arrow key is pressed...
        else if (key == KeyEvent.VK_LEFT){
            //...the player moves left.
            body.startWalking(-WALKING_SPEED);
        }


    }

    /**
     * This method is executed every time a key is released from being pressed on the user's keyboard in the over world levels.
     * This allows the player to stop moving when they let go of the movement keys
     * @param e The event of a key being released
     */
    @Override
    public void keyReleased(KeyEvent e) {
        int key = e.getKeyCode();
        //If the right arrow key is released...
        if (key == KeyEvent.VK_RIGHT) {
            //...the player stops moving.
            body.startWalking(0);
        }
        //If the left arrow key is released...
        else if (key == KeyEvent.VK_LEFT) {
            //...the player stops moving
            body.startWalking(0);
        }
        //If the up arrow key is released...
        else if (key == KeyEvent.VK_UP) {
            //...the player stops moving.
            body.setLinearVelocity(new Vec2(0,0));
        }
        //If the down arrow key is released...
        else if (key == KeyEvent.VK_DOWN) {
            //...the player stops moving.
            body.setLinearVelocity(new Vec2(0, 0));
        }
    }

    //Method to return the player body. This is mainly so that the controller can be assigned to the player body
    public void setBody(Walker body) {
        this.body = body;
    }
}