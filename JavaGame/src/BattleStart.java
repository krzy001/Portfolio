import city.cs.engine.*;

/**
 * This class is for a battle being initiated between an enemy and the layer
 */

public class BattleStart implements CollisionListener {
    private Game game;
    private Player player;
    private CharacterBattle enemyStats;

    private Battle battle;

    public BattleStart(Game game, Player player, CharacterBattle enemyStats){
        this.game = game;
        this.player = player;
        this.enemyStats = enemyStats;
    }

    /**This method runs every time an enemy in the over world has collides with another body
     * This allows the player to enter the battle world with the enemy when they collide with them
     * @param collisionEvent Any collision that has occurred between the body of the enemy and another body
     */
    @Override
    public void collide(CollisionEvent collisionEvent) {

        //If the enemy has collided with the player's body...
        if (collisionEvent.getOtherBody() == player){
            //A battle starts. The battle world is created.
            battle = new Battle(game, enemyStats);

            //Debug view
            //JFrame debugView = new DebugViewer(battle, 600, 600);

            /* Calling the class in game to stop the level world, set the view to the battle world and
            set the controls */
            game.battleStart(battle.getPlayer(), battle.getEnemy(), enemyStats, battle);

            //battle world begins
            battle.start();

            /*removing the enemy body from the over world so that when the player returns the enemy is
            gone as it would have been defeated by the player */
            collisionEvent.getReportingBody().destroy();

            //call the class to add one more enemy defeated for that level
            player.addOne();
        }
    }
}
