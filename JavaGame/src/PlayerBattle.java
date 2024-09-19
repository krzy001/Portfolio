import city.cs.engine.*;

/**
 * An extension of the CharacterBattle class, this class is exclusive to the player as only the player is capable of
 * earning exp and leveling up.
 */

public class PlayerBattle extends CharacterBattle{
    private static final Shape battleShape = new PolygonShape(-1.0f,2.18f, 1.26f,2.18f, 1.26f,-3.14f, -1.0f,-3.14f);
    private static final BodyImage battleImage = new BodyImage("data/player.gif", 10);
    private static final int begHealth = 100;
    private static final int begAtt = 20;
    int Level = 1;
    int EXP = 0;

    public int getBegHealth() {
        return begHealth;
    }

    public int getBegAtt() {
        return begAtt;
    }

    public int getBegLevel(){
        return 1;
    }

    public int getBegEXP(){
        return 0;
    }

    public int getEXP() {
        return EXP;
    }

    public void setEXP(int EXP) {
        this.EXP = EXP;
    }

    /**
     * Every time the player defeats an enemy they earn exp and with enough exp they become more powerful with increased
     * health and attack
     * @param newEXP The EXP earned from the enemy to be given to the player
     */
    public void gainedEXP(int newEXP) {
        EXP+=newEXP;
        if (EXP/(25*Level) >= 1){
            this.Level+=1;
            this.Attack+= 15;
            this.maxHealth+= 30;
            EXP = 0;
            System.out.println("Level Up!");
        }
    }

    public int getLevel() {
        return Level;
    }

    public void setLevel(int level) {
        Level = level;
    }

    public PlayerBattle() {
        super(begHealth, begAtt, battleShape, battleImage);
    }
}
