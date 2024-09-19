import city.cs.engine.*;

/**
 *A class for the statistics of every character, enemies and the player's stats is in another class but is an extension
 *of this class.
 *Every character must have a HP stat, attack stat, a shape and an image for the battles.
 *Some enemies will also be attacking by throwing knives, so they an attack speed and attack rate to determine the
 *speed of the knives thrown and how often they are thrown respectively.
 */

public class CharacterBattle {
    public int maxHealth;
    public int Health;
    public int Attack;
    public int EXPWorth;
    public float speed;
    public boolean attackThrowing = false;
    public int attSpeed;
    public int attRate;
    public Shape shape;
    public BodyImage image;

    //Getters and setters for the necessary variables for other classes to use

    public boolean isAttackThrowing() {
        return attackThrowing;
    }

    public int getAttSpeed() {
        return attSpeed;
    }

    public int getAttRate() {
        return attRate;
    }

    public int getEXPWorth() {
        return EXPWorth;
    }

    public void setEXPWorth(int EXPWorth) {
        this.EXPWorth = EXPWorth;
    }

    public float getSpeed() {
        return speed;
    }

    public void setSpeed(float speed) {
        this.speed = speed;
    }

    public int getMaxHealth() {
        return maxHealth;
    }

    public void setMaxHealth(int maxHealth) {
        this.maxHealth = maxHealth;
    }

    public void RestoreHealth() {
        Health = this.maxHealth;
    }

    public int getHealth() {
        return Health;
    }

    public void removeHealth(int att) {
        Health -= att;
    }

    public int getAttack() {
        return Attack;
    }

    public void setAttack(int attack) {
        Attack = attack;
    }

    public BodyImage getImage() {
        return image;
    }

    public Shape getShape() {
        return shape;
    }

    /*Two different constructors for the characters that don't throw knives like some bosses and the player, and for
    the enemies that do. */
    public CharacterBattle(int hp, int att, Shape shape, BodyImage image) {
        this.maxHealth = hp;
        this.Health = hp;
        this.Attack = att;
        this.shape = shape;
        this.image = image;
    }

    public CharacterBattle(int hp, int att, Shape shape, BodyImage image, int attSpeed, int attRate) {
        this.maxHealth = hp;
        this.Health = hp;
        this.Attack = att;
        this.shape = shape;
        this.image = image;
        this.attSpeed = attSpeed;
        this.attRate = attRate;
        this.attackThrowing = true;
    }
}
