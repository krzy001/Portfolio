import city.cs.engine.*;
import org.jbox2d.common.Vec2;

/**
 * The class for the fourth level of the game.
 */

public class Level4 extends GameLevel {

    private static final int NUM_ENEMIES = 2;

    private static final Shape enemyShape = new PolygonShape(-1.15f,-4.46f, -2.3f,-1.98f, -2.32f,-0.74f, -0.58f,0.67f, 0.85f,0.67f, 2.36f,-1.15f, 1.58f,-4.45f);
    private static final BodyImage enemyImage = new BodyImage("data/chocobo_enemy.gif", 9f);

    private static final Shape enemyBattleShape = new PolygonShape(-1.56f,-5.88f, -3.12f,-2.74f, -3.12f,-0.96f, -0.72f,1.01f, 1.22f,0.98f, 3.17f,-1.73f, 2.18f,-5.9f);
    private static final BodyImage enemyBattleImage = new BodyImage("data/chocobo_enemy.gif", 12f);

    public void populate(Game game) {
        super.populate(game);
        setGravity(0);

        //Establishing the enemies and their statistics
        CharacterBattle chocoboEnemy = new CharacterBattle(450,120, enemyBattleShape, enemyBattleImage,35,820);
        chocoboEnemy.setEXPWorth(150);
        chocoboEnemy.setSpeed(-9);


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

        Shape BlockShape = new BoxShape(14,1.5f);

        Body Block1 = new StaticBody(this,BlockShape);
        Block1.setPosition(new Vec2(2.5f,-7.5f));

        Body Block2 = new StaticBody(this,BlockShape);
        Block2.setAngleDegrees(90);
        Block2.setPosition(new Vec2(-10,5));

        Shape chocoboShape = new PolygonShape(-1.4f,-2.41f, -2.51f,0.2f, -2.52f,1.65f, -1.58f,2.47f, 2.42f,2.03f, 2.49f,-0.31f, 1.79f,-2.39f);
        for (int i = 0; i<4;i++) {
            Walker chocobo = new Walker(this, chocoboShape);
            chocobo.addImage(new BodyImage("data/chocobo.gif", 5f));
            chocobo.setPosition(new Vec2(15, -4+i*5f));
            chocobo.startWalking(-3);
        }

        for (int i = 0; i < (NUM_ENEMIES - this.getPlayer().getCount()); i++) {
            Walker enemy = new Character(this, enemyShape, enemyImage);
            enemy.setPosition(new Vec2(12+i, -12+i*3));
            enemy.addCollisionListener(new BattleStart(game, this.getPlayer(), chocoboEnemy));
            enemy.startWalking(-7);
        }
    }


    @Override
    public Vec2 startPosition() {
        return new Vec2(0,-13f);
    }

    @Override
    public Vec2 exitPosition() {
        return new Vec2(-14,14.5f);
    }

    @Override
    public boolean isCompleted() {
        if (this.getPlayer().getCount() >= NUM_ENEMIES-1){
            return true;
        } else{
            return false;
        }
    }

    @Override
    public int getLevelNumber() {
        return 4;
    }
}
