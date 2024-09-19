import city.cs.engine.*;

/**
 * The class for the body of the player in the over world levels
 */

public class Player extends Walker {
    //The shape and image of the player in the "over world" (where the player will interact in the top-down perspective)
    private static final Shape shape = new PolygonShape(-0.62f,1.42f, 0.82f,1.42f, 0.82f,-2.07f, -0.62f,-2.07f);
    private static final BodyImage image = new BodyImage("data/player.gif", 6.5f);

    private int enemyCount;

    public Player(World world){
        super(world, shape);
        addImage(image);
    }

    public Player(World world, Shape shape, BodyImage image){
        super(world, shape);
        addImage(image);
    }

    public int getCount() {
        return enemyCount;
    }

    public void setEnemyCount(int enemyCount) {
        this.enemyCount = enemyCount;
    }

    public void addOne() {
        enemyCount++;
    }

    public void resetCount() {
        enemyCount = 0;
    }
}