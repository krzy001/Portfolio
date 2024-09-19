import city.cs.engine.*;

import javax.swing.*;
import java.awt.*;

/**
 * This class is essentially where the entire environment of the game is as it holds and has access to most of the very
 * important variables and other classes. As a result, this class contains most of the very important methods
 */

public class Game {

    private GameLevel world;
    private World battleWorld;

    private final JFrame frame = new JFrame("RPG");
    private StatsView view;

    private int level;
    private boolean battle ;
    private int ENEMY_KILLED = 0;

    private ControllerOverWorld controllerOverWorld;
    private ControllerBattle controllerBattle;

    PlayerBattle playerStats = new PlayerBattle();

    /**
     * This is where the game is set up when the user runs it
     * A new world is created starting at level 1 and it is populated
     * The GUI is set up for the user to interact with at the bottom of the screen
     * The controller is also set up for the user to be able to control the player
     */
    public Game(){
        battle = false;
        level = 1;
        world = new Level1();
        world.populate(this);

        view = new StatsView(world,this, 650,650);
        frame.add(view);
        controllerOverWorld = new ControllerOverWorld(world.getPlayer());
        frame.addKeyListener(controllerOverWorld);

        ControlPanel buttons = new ControlPanel(this);
        frame.add(buttons.getMainPanel(), BorderLayout.SOUTH);

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationByPlatform(true);
        frame.setResizable(false);
        frame.pack();
        frame.setVisible(true);
        frame.requestFocus();

        //JFrame debugView = new DebugViewer(world, 600, 600);

        world.start();
    }

    /**
     *
     * @return contains the GUI buttons and the view of the current world. It is the window
     */
    public JFrame getFrame(){
        return frame;
    }

    /**
     *
     * @return The player in the current level world
     */
    public Player getPlayer() {
        return world.getPlayer();
    }

    /**
     *
     * @return The current world
     */
    public World getWorld() {
        return world;
    }

    /**
     *
     * @return The current level number
     */
    public int getLevel(){
        return level;
    }

    /**
     *
     * @return Whether or not the requirements have been met for the player to proceed to the next level
     */
    public boolean isCurrentLevelCompleted(){
        return world.isCompleted();
    }

    /**
     * This method places the player into the next level
     * It checks what level the player is currently in
     * If the player is in the last level then the game ends
     * If the player is in any other level then the player is placed in the next level with the next level needing
     * to be populated, the controller being set on the new body od that level and the view being set to that level
     * With all that the level can start
     */
    public void goNextLevel() {
        world.stop();
        ENEMY_KILLED = 0;
        if (level == 5) {
            System.exit(0);
        } else if (level == 1) {
            world = new Level2();
        } else if (level == 2) {
            world = new Level3();
        } else if (level == 3) {
            world = new Level4();
        } else if (level == 4){
            world = new Level5();
        }
        level++;
        world.populate(this);
        controllerOverWorld.setBody(world.getPlayer());
        view.setWorld(world);
        world.start();
    }

    /**
     * This method runs every time the user loads or resets the game
     * It essentially works the same as the method of going to the next level except it uses the level that it has
     * received as a parameter and it doesn't need to check what level the player is currently in
     * It also receives the number of enemies killed in that level so that it can be used to deduct the number of
     * enemies in that level when the level is loaded
     * This prevents enemies from being respawned after being defeated
     * @param currentLevel The level the game is going to load to
     * @param ENEMY_KILLED The number of enemies that must be removed from that level
     */
    public void goToLevel(GameLevel currentLevel, int ENEMY_KILLED){
        this.ENEMY_KILLED = ENEMY_KILLED;
        world.stop();
        level = currentLevel.getLevelNumber();
        battle = false;
        world = currentLevel;
        world.populate(this);
        controllerOverWorld.setBody(world.getPlayer());
        view.setWorld(world);
        world.start();
    }

    /**
     * This method runs every time a battle is about to start
     * It sets the view into the battle world and pauses the current level to ensure nothing in the level world
     * occurs during the fight. The battle controller is set to the player
     * @param player The player body for the battle world
     * @param enemy The enemy body for the battle world
     * @param enemyStats The enemy's stats
     * @param battleWorld The battle world itself
     */
    public void battleStart(Player player, Character enemy, CharacterBattle enemyStats, World battleWorld){
        this.battleWorld = battleWorld;
        view.setWorld(battleWorld);
        this.getWorld().stop();
        controllerBattle = new ControllerBattle(this, player, enemy, enemyStats, battleWorld);
        frame.addKeyListener(controllerBattle);
        battle = true;
    }

    /**
     *
     * @return the current battle world the player is in
     */
    public World getBattleWorld(){
        return battleWorld;
    }

    /**
     * The method for when the player has defeated an enemy. The player's health is restored, the view is set back to
     * the level world and the controller is given to the player again and the level resumes from where the player left
     * off with the enemy now gone
     */
    public void battleWon() {
        playerStats.RestoreHealth();
        view.setWorld(world);
        controllerOverWorld.setBody(world.getPlayer());
        battle = false;
        world.start();
    }

    /**
     * When the player lost the battle, the player gets removed from the world and the player loses control over the
     * player
     */
    public void battleLost() {
        System.out.println("Battle Lost");
        world.getPlayer().destroy();
        frame.removeKeyListener(controllerBattle);
    }

    /**
     *
     * @return whether or not the player is currently in a battle
     */
    public boolean getBattle(){
        return battle;
    }

    /**
     *
     * @return the number of enemies the player has defeated in that world
     */
    public int getENEMY_KILLED() {
        return ENEMY_KILLED;
    }

    /**
     *
     * @return The player's stats
     */
    public PlayerBattle getPlayerStats(){
        return playerStats;
    }

    /**
     *
     * @param args The main method
     */
    public static void main(String[] args) {
        new Game();
    }
}