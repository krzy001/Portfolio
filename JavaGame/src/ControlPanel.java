import city.cs.engine.*;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

/**
 * This class is for the GUI at the bottom of the screen. It allows the player to pause and resume the game, save and
 * load all their game progress, reset the game, and quit the game.
 */

public class ControlPanel {
    private Game game;
    private World world;
    private JPanel mainPanel;
    private JButton PAUSEPLAYButton;
    private JButton QUITButton;
    private JButton SAVEButton;
    private JButton LOADButton;
    private JButton RESETButton;

    public ControlPanel(Game game) {
        this.game = game;
        PAUSEPLAYButton.addActionListener(new ActionListener() {
            /**
             * This method is for the PAUSE/PLAY button
             * If the player is in the battle world, the battle world gets paused or played. If the player is in the level
             * world, the level world is paused or played
             * If the world is paused, it plays. If the world is playing, it is paused
             * The worlds have been handled in this way because the battle world can exist simultaneously alongside the
             * the level world so it needs to know what world the player is currently interacting with
             * @param e When the button has been pressed
             */
            @Override
            public void actionPerformed(ActionEvent e) {
                if (game.getBattle()){
                    world = game.getBattleWorld();
                } else if (!game.getBattle()){
                    world = game.getWorld();
                }

                if (world.isRunning()) {
                    world.stop();
                } else if (!world.isRunning()) {
                    game.getFrame().requestFocus();
                    world.start();
                } else {
                    game.getFrame().requestFocus();
                }
            }
        });
        QUITButton.addActionListener(new ActionListener() {
            /**
             * This method is for the QUIT button
             * Simply allows the user to close the game
             * @param e When the button has been pressed
             */
            @Override
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });

        SAVEButton.addActionListener(new ActionListener() {
            /**
             * This method is for the SAVE button
             * Information is fed into the writeSave method of the GameSaver class for the file to be saved
             * The current level the player is in, the position of the player in the level,their maximum health,
             * their attack, their level, their experience points and the number of enemies they defeated in the level
             * are all saved for the user to load in the future
             * @param e When the button has been pressed
             */
            @Override
            public void actionPerformed(ActionEvent e) {
                if (!game.getBattle()) {
                    GameSaver save = new GameSaver("data/SaveFile.txt");
                    GameLevel currentLevel;
                    Player player;
                    PlayerBattle stats = game.getPlayerStats();
                    currentLevel = (GameLevel) game.getWorld();
                    player = currentLevel.getPlayer();

                    try {
                        save.writeSave(game.getLevel(),
                                player.getPosition().x,
                                player.getPosition().y,
                                stats.getMaxHealth(),
                                stats.getAttack(),
                                stats.getLevel(),
                                stats.getEXP(),
                                player.getCount());
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }
                } else {
                    System.out.println("Unable to save during battles");
                }
                game.getFrame().requestFocus();
            }
        });

        LOADButton.addActionListener(new ActionListener() {
            /**
             * This method is for the LOAD button
             * It allows to user to load the save file and continue the game from their saved progress
             * Everything that is necessary to continue the game is loaded
             * @param e When the button has been pressed
             */
            @Override
            public void actionPerformed(ActionEvent e) {
                GameLoader load = new GameLoader("data/SaveFile.txt", game);
                try{
                    load.readSave();
                }
                catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        });
        RESETButton.addActionListener(new ActionListener() {
            /**
             * This method is for the RESET button
             * It allows the user to reset the game and start from the beginning with all of their beginning stats,
             * which are all retrieved from the PlayerBattle class via the Game class
             * @param e When the button has been pressed
             */
            @Override
            public void actionPerformed(ActionEvent e) {
                PlayerBattle stats = game.getPlayerStats();

                stats.setMaxHealth(stats.getBegHealth());
                stats.RestoreHealth();
                stats.setAttack(stats.getBegAtt());
                stats.setLevel(stats.getBegLevel());
                stats.setEXP(stats.getBegEXP());

                game.goToLevel(new Level1(), 0);
            }
        });
    }

    /**
     *
     * @return The panel containing all the GUI buttons
     */
    public JPanel getMainPanel() {
        return mainPanel;
    }
}
