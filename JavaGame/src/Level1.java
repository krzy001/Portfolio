import city.cs.engine.*;
import org.jbox2d.common.Vec2;

/**
 * The class for the first level of the game.
 */

public class Level1 extends GameLevel {

    private static final int NUM_ENEMIES = 3;

    private static final Shape enemyShape = new PolygonShape(-0.62f,1.42f, 0.82f,1.42f, 0.82f,-2.07f, -0.62f,-2.07f);
    private static final BodyImage enemyImage = new BodyImage("data/guard.gif", 6.5f);

    private static final Shape enemyBattleShape = new PolygonShape(-1.0f,2.18f, 1.26f,2.18f, 1.26f,-3.14f, -1.0f,-3.14f);
    private static final BodyImage enemyBattleImage = new BodyImage("data/guard.gif", 10f);

    @Override
    public void populate(Game game){
        super.populate(game);
        setGravity(0);
        game.getFrame().requestFocus();

        //Establishing the enemies and their statistics
        CharacterBattle Guard = new CharacterBattle(130,20, enemyBattleShape, enemyBattleImage, 25, 1000);
        Guard.setEXPWorth(25);
        Guard.setSpeed(0);

        //Establishing borders as walls to prevent player from going out of bounds
        //Shape of the wall length of on side of the screen
        Shape WallShape = new BoxShape(1.5f,17);

        //The wall to the east side of the screen will be visible to the player
        Body WallEast = new StaticBody(this,WallShape);
        WallEast.setPosition(new Vec2(15,0));

        //The West and South walls are made to be just out of sight so the player can't see them
        Body WallWest = new StaticBody(this,WallShape);
        WallWest.setPosition(new Vec2(-17.5f,0));

        //Rotating the shape of the wall by 90 degrees for it to go along the south of the screen
        Body WallSouth = new StaticBody(this,WallShape);
        WallSouth.setAngleDegrees(90);
        WallSouth.setPosition(new Vec2(0,-17.5f));

        Body WallNorth = new StaticBody(this,WallShape);
        WallNorth.setAngleDegrees(90);
        WallNorth.setPosition(new Vec2(6,17.5f));

        //Setting up the environment for the player to interact in

        // The prison walls are made to be static bodies so that they can't be moved
        Shape prisonWall = new BoxShape(1.5f,15);
        Body prisonWall1 = new StaticBody(this,prisonWall);
        Body prisonWall2 = new StaticBody(this,prisonWall);
        prisonWall1.setPosition(new Vec2(3,12));
        prisonWall2.setPosition(new Vec2(-9,12));


        Shape prisonBlock = new BoxShape(4,1f);
        Body prisonBlock1 = new StaticBody(this,(prisonBlock));
        prisonBlock1.setPosition(new Vec2(7,8));

        Body prisonBlock2 = new StaticBody(this,prisonBlock);
        prisonBlock2.setPosition(new Vec2(-5,8));


        //An obstacle the player would have to "kick" open to proceed
        //Used the polygon editor to create the shape of the rock with coordinates
        Shape rock = new PolygonShape(-0.984f,0.631f, 0.288f,1.261f, 1.133f,0.852f, 1.454f,-0.121f, 1.139f,-1.199f, -1.178f,-1.177f, -1.344f,0.072f);
        Body Boulder = new DynamicBody(this, rock);
        Boulder.addImage(new BodyImage("data/rock.png",2.25f));
        Boulder.setPosition(new Vec2(12.25f,8f));

        //Spawning three of the same enemies in different locations of the level
        for (int i = 0; i < (NUM_ENEMIES - this.getPlayer().getCount()); i++) {
            Walker guard = new Character(this, enemyShape, enemyImage);
            guard.setPosition(new Vec2(0, -1-(i*5)));
            guard.addCollisionListener(new BattleStart(game, this.getPlayer(), Guard));
            guard.startWalking(3);
        }
    }

    @Override
    public Vec2 startPosition() {
        return new Vec2(8, 13);
    }

    @Override
    public Vec2 exitPosition() {
        return new Vec2(-13.4f, 14.5f);
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
        return 1;
    }
}