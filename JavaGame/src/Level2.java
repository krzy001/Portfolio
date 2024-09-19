import city.cs.engine.*;
import org.jbox2d.common.Vec2;

/**
 * The class for the second level of the game.
 */

public class Level2 extends GameLevel {

    private static final int NUM_ENEMIES = 5;
    private static final int NUM_TREES = 5;

    private static final Shape enemyShape = new PolygonShape(-0.62f,1.42f, 0.82f,1.42f, 0.82f,-2.07f, -0.62f,-2.07f);
    private static final BodyImage enemyImage = new BodyImage("data/guard.gif", 6.5f);

    private static final Shape enemyBattleShape = new PolygonShape(-1.0f,2.18f, 1.26f,2.18f, 1.26f,-3.14f, -1.0f,-3.14f);
    private static final BodyImage enemyBattleImage = new BodyImage("data/guard.gif", 10f);

    public void populate(Game game) {
        super.populate(game);
        setGravity(0);

        //Establishing the enemies and their statistics
        CharacterBattle Guard = new CharacterBattle(300,45, enemyBattleShape, enemyBattleImage, 30, 900);
        Guard.setEXPWorth(50);
        Guard.setSpeed(-5);

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
        WallSouth.setPosition(new Vec2(0,-17.5f));

        Body WallNorth = new StaticBody(this, WallShape);
        WallNorth.setAngleDegrees(90);
        WallNorth.setPosition(new Vec2(-9,17.5f));

        Shape treeShape = new PolygonShape(-0.89f,-2.34f, -2.31f,0.07f, -0.05f,1.94f, 2.11f,0.09f, 0.89f,-2.42f);
        for(int i = 0; i < NUM_TREES-1; i++) {
            for(int j = 0; j < NUM_TREES; j++) {
                Body Tree = new StaticBody(this, treeShape);
                Tree.addImage(new BodyImage("data/tree.png", 5f));
                Tree.setPosition(new Vec2(-15+i*8f, -12+j*8f));
            }
        }

        for (int i = 0; i < (NUM_ENEMIES - this.getPlayer().getCount()); i++) {
                Walker guard = new Character(this, enemyShape, enemyImage);
                guard.setPosition(new Vec2(-8+i*4, 10-i*4));
                guard.addCollisionListener(new BattleStart(game, this.getPlayer(), Guard));
                guard.startWalking(-1);
                guard.jump(-10);
        }
    }

    @Override
    public Vec2 startPosition() {
        return new Vec2(-13, -14);
    }

    @Override
    public Vec2 exitPosition() {
        return new Vec2(11,14.5f);
    }

    @Override
    public boolean isCompleted() {
        if (this.getPlayer().getCount() >= NUM_ENEMIES-2){
            return true;
        } else{
            return false;
        }
    }

    @Override
    public int getLevelNumber() {
        return 2;
    }
}