import city.cs.engine.CollisionEvent;
import city.cs.engine.CollisionListener;
import city.cs.engine.*;

/**
 * This class establishes what happens when a knife from the enemy collides with the player
 */
public class EnemyAttackRules implements CollisionListener {

    private Game game;
    private Walker body;
    private CharacterBattle enemyStats;

    public EnemyAttackRules(Game game, Walker body, CharacterBattle enemyStats) {
        this.game = game;
        this.body = body;
        this.enemyStats = enemyStats;
    }

    /**
     * This method runs every time an enemy's knife weapon collides with another body
     * This allows the knife to damage the player and remove health with the enemy's attack stat.
     * @param collisionEvent collision between the knife from the enemy and another body
     */
    @Override
    public void collide(CollisionEvent collisionEvent) {
        //If the knife collides with the player body...
        if (collisionEvent.getOtherBody() == body){
            //...the player's health gets removed with the enemy's attack stat
            game.getPlayerStats().removeHealth(enemyStats.getAttack());

            //Remove the knife from the world
            collisionEvent.getReportingBody().destroy();

            //If the player's health has reached or passed 0 as a result...
            if (game.getPlayerStats().getHealth() <= 0) {
                //...the game is over, so the player body is removed and the world is stopped.
                body.destroy();
                game.battleLost();
                game.getBattleWorld().stop();
            }
        }
    }
}
