import city.cs.engine.*;
import org.jbox2d.common.Vec2;

/**
 * The class for the final level of the game.
 */

public class Level5 extends GameLevel {

    private static final int NUM_ENEMIES = 1;

    private static final Shape enemyShape = new PolygonShape(-1.34f,-2.0f, -1.34f,2.01f, 1.33f,1.99f, 1.35f,-2.01f);
    private static final BodyImage enemyImage = new BodyImage("data/final_boss.gif", 4f);

    private static final Shape enemyBattleShape = new PolygonShape(-0.98f,-2.9f, -2.62f,-1.27f, -2.9f,2.82f, -1.44f,3.0f, 2.18f,2.69f, 3.01f,-1.18f, 1.5f,-3.01f);
    private static final BodyImage enemyBattleImage = new BodyImage("data/final_boss.png", 6f);

    public void populate(Game game) {
        super.populate(game);
        setGravity(0);

        //Establishing the enemies and their statistics
        CharacterBattle finalBoss = new CharacterBattle(5000,300, enemyBattleShape, enemyBattleImage, 8, 1800);
        finalBoss.setEXPWorth(200);
        finalBoss.setSpeed(-1.25f);

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
            Walker guard = new Character(this, enemyShape, enemyImage);
            guard.setPosition(new Vec2(0, -2));
            guard.addCollisionListener(new BattleStart(game, this.getPlayer(), finalBoss));
        }
    }

    @Override
    public Vec2 startPosition() {
        return new Vec2(-14,-16.5f);
    }

    @Override
    public Vec2 exitPosition() {
        return new Vec2(0,14.5f);
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
        return 5;
    }
}
