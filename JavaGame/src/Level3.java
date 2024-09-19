import city.cs.engine.*;
import org.jbox2d.common.Vec2;

/**
 * The class for the third level of the game.
 */

public class Level3 extends GameLevel {

    private static final int NUM_ENEMIES = 1;

    private static final Shape enemyShape = new PolygonShape(-0.62f,1.42f, 0.82f,1.42f, 0.82f,-2.07f, -0.62f,-2.07f);
    private static final BodyImage enemyImage = new BodyImage("data/tonberry.gif", 4f);

    private static final Shape enemyBattleShape = new PolygonShape(-1.0f,2.18f, 1.26f,2.18f, 1.26f,-3.14f, -1.0f,-3.14f);
    private static final BodyImage enemyBattleImage = new BodyImage("data/tonberry_attack.png", 6f);

    public void populate(Game game) {
        super.populate(game);
        setGravity(0);

        //Establishing the enemies and their statistics
        CharacterBattle tonberryBoss = new CharacterBattle(425,200, enemyBattleShape, enemyBattleImage);
        tonberryBoss.setEXPWorth(200);
        tonberryBoss.setSpeed(-13);

        Shape WallShape = new BoxShape(1.5f, 17);

        //The wall to the east side of the screen will be visible to the player
        Body WallEast = new StaticBody(this, WallShape);
        WallEast.setPosition(new Vec2(18, 0));

        //The West and South walls are made to be just out of sight so the player can't see them
        Body WallWest = new StaticBody(this, WallShape);
        WallWest.setPosition(new Vec2(-18f, 0));

        //Rotating the shape of the wall by 90 degrees for it to go along the south of the screen
        Body WallSouth = new StaticBody(this, WallShape);
        WallSouth.setAngleDegrees(90);
        WallSouth.setPosition(new Vec2(0,-18));

        Body WallNorth = new StaticBody(this, WallShape);
        WallNorth.setAngleDegrees(90);
        WallNorth.setPosition(new Vec2(0,18f));


        for (int i = 0; i < (NUM_ENEMIES - this.getPlayer().getCount()); i++) {
            Walker enemy = new Character(this, enemyShape, enemyImage);
            enemy.setPosition(new Vec2(0, 8));
            enemy.addCollisionListener(new BattleStart(game, this.getPlayer(), tonberryBoss));
        }

    }

    @Override
    public Vec2 startPosition() {
        return new Vec2(13, -14);
    }

    @Override
    public Vec2 exitPosition() {
        return new Vec2(0,12.5f);
    }

    @Override
    public boolean isCompleted() {
        if (this.getPlayer().getCount() >= NUM_ENEMIES){
            return true;
        } else {
            return false;
        }
    }

    @Override
    public int getLevelNumber() {
        return 3;
    }
}
