import city.cs.engine.UserView;
import city.cs.engine.*;

import javax.swing.*;
import java.awt.*;

/**
 * This class is for the view of all the worlds and levels and allows the user to see their health and attack stats.
 * It also allows each level to have unique backgrounds.
 */

public class StatsView extends UserView {

    private Game game;

    public StatsView(World world, Game game, int width, int height) {
        super(world, width, height);
        this.game = game;
    }

    /**
     * This method is to display the player's current statistics in the top left corner of the screen so that it
     * doesn't obstruct the gameplay.
     */
    @Override
    protected void paintForeground(Graphics2D g) {
        g.setFont(new Font("TimesRoman", Font.BOLD, 20));
        g.drawString("Level: " + game.getPlayerStats().getLevel(), 10, 25);
        g.drawString("Health: " + game.getPlayerStats().getHealth(), 10, 45);
        g.drawString("Attack: " + game.getPlayerStats().getAttack(), 10,65);
    }

    /**
     * This method paints the background of every stage and also the battle screen. If the player isn't in a battle,
     * the background will be displayed and the background will be different depending on what level the player is
     * is currently in.
     * If the player is in a battle, the background will be a "FIGHT" screen.
     */
    @Override
    protected void paintBackground(Graphics2D g) {
        super.paintBackground(g);

        if(!game.getBattle()) {
            Image backgroundImage = new ImageIcon("data/background" + game.getLevel() + ".jpg").getImage();
            g.drawImage(backgroundImage, 0, 0, null);
        } else if(game.getBattle()){
            Image backgroundImage = new ImageIcon("data/battle.png").getImage();
            g.drawImage(backgroundImage, 5, 0, null);
        }
    }
}