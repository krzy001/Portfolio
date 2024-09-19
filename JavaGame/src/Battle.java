import city.cs.engine.*;
import city.cs.engine.Shape;
import org.jbox2d.common.Vec2;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * This is essentially the "battlefield" for the player for whenever they have to face an enemy
 * The perspective is now to the side, gravity is in place allowing the player to jump and they have
 * to try defeat the enemy before they defeat the player
 * The player attacks by shooting bullets at the enemy and the enemy attacks by mainly throwing knives at the
 * player or touching the player
 */

public class Battle extends World implements ActionListener {
    private Game game;
    private Player player;
    private Character opponent;
    private CharacterBattle enemyStats;
    private Timer timer;
    private DynamicBody enemyAttack;

    /**
     *
     * @param game The game class which has access to most of the important methods and variables of the game
     * @param enemyStats The enemy's stats
     */
    public Battle(Game game, CharacterBattle enemyStats){

        this.game = game;
        this.enemyStats = enemyStats;

        /* The walls are made to be just out of sight so the player can't see them.
        The purpose of these walls is to prevent the player from going out of bounds off the screen
         */

        Shape Wall = new BoxShape(1.5f,17);

        Body WallEast = new StaticBody(this,Wall);
        WallEast.setPosition(new Vec2(17.5f,0));

        Body WallWest = new StaticBody(this,Wall);
        WallWest.setPosition(new Vec2(-17.5f,0));

        //Rotating the shape of the wall by 90 degrees for it to go along the south of the screen
        Body Floor = new StaticBody(this,Wall);
        Floor.setAngleDegrees(90);
        Floor.setPosition(new Vec2(0,-17.5f));

        // Creating the player and opponent into the battle world.
        player = new Player(this, game.getPlayerStats().getShape(), game.getPlayerStats().getImage());
        player.setPosition(new Vec2(-8, -12));

        opponent = new Character(this, enemyStats.getShape(), enemyStats.getImage());
        opponent. setPosition(new Vec2(8,-12));

        /*Calling a class to establish what happens when the enemy makes contact with the player.
        The player would take damage and lose health
         */
        opponent.addCollisionListener(new BattleRules(game, player, enemyStats));

        //opponent starts walking towards the player
        opponent.startWalking(enemyStats.getSpeed());

        /*
        If the enemy attacks by throwing knives, then set up the timer of the rate at which the enemy attacks.
         */
        if (enemyStats.isAttackThrowing()) {
            timer = new Timer(enemyStats.getAttRate(), this);
            timer.setInitialDelay(100);
            timer.start();
        }
    }

    /**
     *
     * @return For other classes to gain access to the player in the battle world
     */
    public Player getPlayer() {
        return player;
    }

    /**
     *
     * @return For other classes to gain access to the enemy in the battle world
     */
    public Character getEnemy() {
        return opponent;
    }

    /**
     * This method runs at the rate of the timer established earlier
     * A new dynamic body is created in the form of a new knife thrown at the player
     * @param e The timed event
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        //The polygon shape to match the image and the height of the knife
        Shape enemyAttackShape = new PolygonShape(-0.881f,-0.836f, -0.258f,0.618f, 0.802f,0.763f, 0.577f,-0.142f, -0.586f,-0.848f);

        //Dynamic body is created and it uses the polygon shape in this battle world
        enemyAttack = new DynamicBody(this, enemyAttackShape);

        //Add the image onto the dynamic body
        enemyAttack.addImage(new BodyImage("data/knife.png",2f));

        /* Setting the starting position of the knife to be where the enemy is. it is set a little bit to the left
        of the enemy to ensure that the object is able to thrown to the left side of the enemy and towards where
        the player will be */
        enemyAttack.setPosition(new Vec2((opponent.getPosition().x-0.5f),opponent.getPosition().y));

        //Angle of the object so that the sharp point of the knife is facing the player
        enemyAttack.setAngleDegrees(-45);
        enemyAttack.setBullet(true);

        //The velocity of the throwing knife which is dependant on the enemy
        enemyAttack.setLinearVelocity(new Vec2(-enemyStats.getAttSpeed(),0));

        //Calling a class to establish what happens when the knife hits the player
        enemyAttack.addCollisionListener(new EnemyAttackRules(game, player, enemyStats));
    }
}