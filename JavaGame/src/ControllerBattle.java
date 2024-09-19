import city.cs.engine.*;
import org.jbox2d.common.Vec2;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

/**
*The controls for the player in the battle world. They are capable of moving, jumping and shooting in order to defeat
*their enemies.
 */

public class ControllerBattle extends KeyAdapter {
    //Establishing their jumping and walking speed
    private float JUMPING_SPEED = 7;
    private float WALKING_SPEED = 11;

    private Game game;
    private World world;
    private Walker body;
    private Character enemy;
    private CharacterBattle enemyStats;
    private DynamicBody bulletBody;


    public ControllerBattle(Game game, Walker body, Character enemy, CharacterBattle enemyStats, World world){
        this.body = body;
        this.enemy = enemy;
        this.enemyStats = enemyStats;
        this.world = world;
        this.game = game;
    }

    /**
     * This method is executed every time a key is pressed on the user's keyboard in the battle world.
     * This is what allows them jump, move and shoot bullets. The bullets are created within this method
     * when the correct key is pressed.
     * @param e The event of a key being pressed
     */
    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();
        game.getFrame().requestFocus();

        //If the key pressed is "Z"...
        if (key == KeyEvent.VK_Z) {

            //...get the linear velocity of the player and if they are not moving across the y axis...
            Vec2 v = body.getLinearVelocity();
            if (Math.abs(v.y) < 0.01f) {
                //...the player jumps. It is done in this way to prevent the player from jumping when they are already in the air.
                body.jump(JUMPING_SPEED);
            }
        }
        //If the key pressed is "X"...
        else if (key == KeyEvent.VK_X) {
            //...they player shoots a bullet. This is where the dynamic body is created and shot.

            //The polygon shape to match the image and the height of the bullet
            Shape bulletShape = new PolygonShape(0.0f,0.498f, 0.138f,0.352f, 0.168f,0.136f, 0.168f,-0.5f, -0.164f,-0.496f, -0.166f,0.146f, -0.122f,0.36f);

            //Dynamic body is created and it uses the polygon shape in this battle world
            bulletBody = new DynamicBody(world, bulletShape);

            //Add the image onto the dynamic body
            bulletBody.addImage(new BodyImage("data/bullet.png",1f));

            /* Setting the starting position of the bullet to be where the player is. It is set a little bit to the right
            of the player to ensure that the object is able to shot to the right side of the player and towards where
            the enemy would be */
            bulletBody.setPosition(new Vec2((body.getPosition().x+0.5f),body.getPosition().y));

            //Angle of the object so that the bullet is facing the enemy
            bulletBody.setAngleDegrees(-84);
            bulletBody.setBullet(true);

            //The velocity of the bullet
            bulletBody.setLinearVelocity(new Vec2(40,0));

            //Calling a class to establish what happens when the bullet hits the enemy
            bulletBody.addCollisionListener(new BulletRules(game, world, enemy, enemyStats));
        }
        //If the key pressed is the right arrow key...
        else if (key == KeyEvent.VK_RIGHT) {
            //...the player moves to the right
            body.startWalking(WALKING_SPEED);
        }
        //If the key pressed is the left arrow key...
        else if (key == KeyEvent.VK_LEFT){
            //...the player moves to the left
            body.startWalking(-WALKING_SPEED);
        }
    }

    /**
     * This method is executed every time a key is released from being pressed on the user's keyboard in the battle world.
     * Simply lets the player to stop moving when they let go of the movement keys
     * @param e The event of a key being released
     */
    @Override
    public void keyReleased(KeyEvent e) {
        int key = e.getKeyCode();

        //If the right arrow key is released...
        if (key == KeyEvent.VK_RIGHT) {
            //...the player stops walking
            body.startWalking(0);
        }
        //If the left arrow key is released...
        else if (key == KeyEvent.VK_LEFT) {
            //...the player stops walking
            body.startWalking(0);
        }
    }

    /**
     *
     * @param body The body of the player to assign the controller to
     */
    public void setBody(Walker body) {
        this.body = body;
    }
}