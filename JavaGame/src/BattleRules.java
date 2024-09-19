import city.cs.engine.*;

/**
 * This class is for establishing the fact that when the enemy touches the player in the battle world, the player takes
 * damage.
 */

public class BattleRules implements CollisionListener {
    private Game game;
    private Player player;
    private CharacterBattle enemyStats;

    public BattleRules(Game game, Player player, CharacterBattle enemyStats) {
        this.game = game;
        this.player = player;
        this.enemyStats = enemyStats;

    }

    /**
     * This allows the enemy to damage the player by making contact with them
     * @param collisionEvent Any collision that has occurred between the body of the enemy and another body
     */
    @Override
    public void collide(CollisionEvent collisionEvent) {
        //If the enemy body collided with the player body...
        if (collisionEvent.getOtherBody() == player){
            //...remove the player's health with the enemy's attack stat
            game.getPlayerStats().removeHealth(enemyStats.getAttack());

            //If the players health reaches 0 or below...
            if (game.getPlayerStats().getHealth() <= 0) {
                //...the game is over. The world stops and the player gets removed from the screen
                player.destroy();
                game.battleLost();
                game.getBattleWorld().stop();
            }
        }
    }
}
