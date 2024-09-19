import java.io.FileWriter;
import java.io.IOException;

/**
 * A class to allow the user to save all their current progress, including all their stats, the level they are in,
 * their position in the level and the number of enemies they have defeated in that level.
 */

public class GameSaver {

    private String fileName;

    /**
     * This method runs every time the user selects the save button on the GUI
     * It allows the user to save all of their progress, including all their stats, the level they are in,
     * their position in the level and the number of enemies they have defeated in that level.
     * @param fileName The name of the file that is going o be created/overwritten
     */
    public GameSaver(String fileName) {
        this.fileName = fileName;
    }

    public void writeSave(int levelProgress,
                          float positionX,
                          float positionY,
                          int maxHealth,
                          int Attack,
                          int Level,
                          int EXP,
                          int ENEMYKILLED) throws IOException {
        FileWriter writer = null;

        /*Writing all the values with commas in between so that when the file is loaded the values can be split
        into a list via the commas being split
         */
        try {
            writer = new FileWriter(fileName);
            writer.write(levelProgress + "," +
                    positionX + "," +
                    positionY + "," +
                    maxHealth + "," +
                    Attack + "," +
                    Level + "," +
                    EXP + "," +
                    ENEMYKILLED + "\n");
        } finally {
            if (writer != null) {
                writer.close();
            }
        }
    }
}
