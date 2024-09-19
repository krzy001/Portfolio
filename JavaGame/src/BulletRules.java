import city.cs.engine.*;

/**
 * This class is for what happens when a bullet from the player collides with the enemy
 */

public class BulletRules implements CollisionListener {

    private Game game;
    private World world;
    private Character enemy;
    private CharacterBattle enemyStats;

    public BulletRules(Game game, World world, Character enemy, CharacterBattle enemyStats) {
        this.game = game;
        this.world = world;
        this.enemy = enemy;
        this.enemyStats = enemyStats;
    }

    /**
     * This method is called every time a bullet from the player collides with another body
     * This allows the bullet to remove health from the enemy with the player's attack power.
     * @param collisionEvent The collision between the bullet and another body
     */
    @Override
    public void collide(CollisionEvent collisionEvent) {
        //If the bullet collided with the enemy's body...
        if (collisionEvent.getOtherBody() == enemy){
            //...remove health from that enemy with the attack stat of the player
            enemyStats.removeHealth(game.getPlayerStats().getAttack());
            System.out.println(enemyStats.getHealth());

            //The bullet gets removed from the world
            collisionEvent.getReportingBody().destroy();

            //If the enemy's health falls to 0 or below as a result...
            if (enemyStats.getHealth() <= 0) {
                System.out.println("Battle Won");

                //...the enemy is defeated so they are removed from the battle world.
                enemy.destroy();

                //The amount of EXP the enemy was worth is given to the player to strengthen the player's stats
                game.getPlayerStats().gainedEXP(enemyStats.getEXPWorth());

                //health is restored for the next enemies in that level
                enemyStats.RestoreHealth();

                //Battle world has stopped
                world.stop();

                //Class is called to set the view and controller back to the over world level and continue the level
                game.battleWon();
            }
        }
    }
}