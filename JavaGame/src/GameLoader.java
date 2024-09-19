import org.jbox2d.common.Vec2;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/**
 * The class for loading a save file into the game to allow the player to continue from their saved progress
 */

public class GameLoader {

    private String fileName;
    private Game game;

    public GameLoader(String fileName, Game game) {
        this.fileName = fileName;
        this.game = game;
    }

    /**
     * This method runs everytime the player clicks on the load button on the GUI
     * The purpose of this method is to load a save file and let the player continue their saved progress
     * @throws IOException Exception for in case searching for the returns a null and to prevent the game from crashing
     */
    public void readSave() throws IOException {
        FileReader fr = null;
        BufferedReader reader = null;
        try {
            System.out.println("Reading " + fileName + " ...");
            fr = new FileReader(fileName);
            reader = new BufferedReader(fr);
            String line = reader.readLine();

            //Taking whats in the file and separating them into separate values by splitting them by the commas in between the values
            String[] tokens = line.split(",");

            //Assigning the separated values from the list into variables
            int levelProgress = Integer.parseInt(tokens[0]);

            float positionX = Float.parseFloat(tokens[1]);
            float positionY = Float.parseFloat(tokens[2]);
            Vec2 posPlayer = new Vec2(positionX, positionY);

            int maxHealth = Integer.parseInt(tokens[3]);
            int Attack = Integer.parseInt(tokens[4]);
            int Level = Integer.parseInt(tokens[5]);
            int EXP = Integer.parseInt(tokens[6]);
            int ENEMY_KILLED = Integer.parseInt(tokens[7]);

            PlayerBattle stats = game.getPlayerStats();

            //assigning those variables into the stats of the player
            stats.setMaxHealth(maxHealth);
            stats.RestoreHealth();
            stats.setAttack(Attack);
            stats.setLevel(Level);
            stats.setEXP(EXP);

            //assigning the correct level the player was in into a variable
            GameLevel currentLevel = null;
            if (levelProgress == 1) {
                currentLevel = new Level1();
            }
            else if (levelProgress == 2) {
                currentLevel = new Level2();
            }
            else if (levelProgress == 3) {
                currentLevel = new Level3();
            }
            else if (levelProgress == 4) {
                currentLevel = new Level4();
            }
            else if (levelProgress == 5) {
                currentLevel = new Level5();
            }

            //Passing the level into a method in game to load the level
            game.goToLevel(currentLevel, ENEMY_KILLED);

            //Place the player in the saved position they were in in the level
            currentLevel.getPlayer().setPosition(posPlayer);

            System.out.println("...done.");


        } finally {
            if (reader != null) {
                reader.close();
            }
            if (fr != null) {
                fr.close();
            }
        }
    }
}
