#pragma once

#include <avr/io.h>
#include "../oled/oled.h"

/**
 * \brief This is a namespace containing everything needed to implement a menu.
 * This namespace contains data types for describing a menu, and a controller class
 * to control it and display it on a OLED (library class).
 * The menu allocates Menu and Item objects on the heap. Thus you should declare
 * these objects ON THE STACK INSIDE A LOCAL SCOPE so the menus and menu items aren't
 * stored both on the heap and in the main (or where the menu is created) stack!
 */
namespace Menu {
    /**
     * \brief Struct containing a sigle menu item.
     * This struct contains all the necessary information
     * about a menu item.
     */
    struct Item {
        /**
         * This is the label that will show up in the menu.
         */
        char *label = nullptr;

        /**
         * This is the length of the label.
         */
        uint8_t label_length;

        /**
         * The action is a pointer to either the cb (callback) function,
         * or it is a pointer to another menu (for instance a sub menu or the parent menu).
         */
        void *action = nullptr;

        /**
         * Decides wether the action points to a sub menu or a cb.
         */
        bool has_sub_menu = false;

        /**
         * This is a pointer to the next menu item in the menu (linked list).
         */
        Item *next = nullptr;
    };

    /**
     * Contains all necessary information about a menu (which could be a sub menu).
     */
    struct Menu {
        /**
         * If it is a sub menu, this points to its parent menu. Else it is nullptr.
         */
        Menu *parent = nullptr;

        /**
         * This is the first menu item in the menu.
         */
        Item *first = nullptr;
    };

    /**
     * This class contains everything needed to create and manage a menu and display it on an OLED.
     */
    class Controller {
    private:
        /**
         * Pointer to the oled object.
         */
        OLED *oled = nullptr;

        /**
         * Specifies how many lines the menu should use (always uses the whole screen).
         */
        uint8_t num_lines;

        /**
         * This is the root menu, in other words the first/main menu.
         */
        Menu *root;


        /**
         * When administrating the menu (control), you use methods to navigate
         * to the item you want to change (e.g. add a menu item).
         * This stores the menu to be controlled.
         */
        Menu *current_menu_control = nullptr;

        /**
         * This stores the current item to be controlled. Should point to an item inside of current_menu_control.
         */
        Item *current_item_control = nullptr;


        /**
         * When using the menu (navigate), you use methods to navigate.
         * This stores the menu you are navigating (on screen).
         */
        Menu *current_menu_navigate = nullptr;

        /**
         * This stores the item needed to for instance render the menu or execute the action of the item.
         */
        Item *current_item_navigate = nullptr;

        /**
         * This stores the index of the first menu item to be displayed on screen.
         * This will change as the user navigates downwards on a menu higher
         * than the number of lines on the screen.
         * When rendering it will be calculated such that the cursor, if possible,
         * does not end up on the first or last lines of the menu. This ensures
         * the user always sees the next item to be selected before it is selected.
         */
        uint8_t current_index_navigate = 0;

        /**
         * This stores the index of the currently selected menu item.
         */
        uint8_t current_index_selected = 0;

        /**
         * This calculates the total length of a menu.
         * It is used to for instance figure out what will be the
         * first displayed menu option so that you cannot try to
         * navigate below the menu.
         * @param menu The menu to calculate the length of.
         * @return The menu length.
         */
        uint8_t GetMenuLength(Menu *menu);

        /**
         * Sets the currently selected navigate item to the item with this item.
         * This will only update the item, not the navigate index. This is because
         * this method is only used to select the item so it can be rendered or its
         * action executed.
         * @param index The index of the item to be selected.
         */
        void GoToItem(uint8_t index);

        /**
         * This will try to set the next item as the navigate item. It does not update the navigate index.
         * @return Whether there was another menu item.
         */
        bool GoToNextItem();
    public:
        /**
         * Sets up the menu.
         * @param oled This is the oled object the menu should use for display.
         * @param num_lines This is the number of lines that the display should use when displaying the menu.
         */
        Controller(OLED &oled, uint8_t num_lines);

        /**
         * Sets the menu to be controlled to root. The item selected will be none.
         */
        void ControlGoToRoot();

        /**
         * Sets the item to be controlled to the item with index index of the selected menu.
         * @param index The index of the item to be selected.
         */
        void ControlGoToItem(uint8_t index);

        /**
         * Enters the sub menu the selected item links to (Item::action). Assumes the current item has a sub menu...
         */
        void ControlEnterSubMenu();

        /**
         * Same as ControlEnterSubMenu, except accesses the menu item with index index
         * instead of the currently selected item.
         * @param index The index of the item whose sub menu will be entered.
         */
        void ControlEnterSubMenu(uint8_t index);

        /**
         * Sets the menu to be controlled to the parent of the current menu. Assumes the currently selected menu
         * isn't the root menu.
         */
        void ControlGoToParent();


        /**
         * Adds a sub menu to the currently selected item. Assumes an item is selected.
         * Internally it creates a menu, and invokes AddMenuItem to add the items. Thus you
         * could create an empty menu and add items using AddMenuItem instead.
         * Do NOT use this to create the root menu; it is already made. Use AddMenuItem to add
         * items to it.
         * @param items An array of items to add to the newly created menu.
         * @param length The length of the array.
         */
        void AddMenu(Item *items, uint8_t length);

        /**
         * Internally invokes AddMenuItem to add the items. Adds to the selected item.
         * @param items An array of items to add to the menu.
         * @param length The length of the array.
         */
        void AddMenuItems(Item *items, uint8_t length);

        /**
         * Adds the item after the currently selected item. If no item is selected, the item
         * is inserted as the first item.
         * @param item The item to insert.
         */
        void AddMenuItem(Item &item);

        /**
         * Adds the item at the specified index. Thus index 0 will place
         * it as the first element in the menu.
         * @param item The item to insert.
         * @param index The index the inserted item should have.
         */
        void AddMenuItem(Item &item, uint8_t index);

        /**
         * Sets the specified menu as the current navigate menu. Thus calling this and rendering the menu
         * will go to this menu.
         * @param menu The menu to go to.
         */
        void GoToMenu(Menu *menu);

        /**
         * Sets the currently selected navigate item to the root. Also updates the
         * navigate index and selected index. This is used to return the navigation to the root.
         * Use this as a handler for navigating to the root in the menu.
         */
        void GoToRoot();

        /**
         * Sets the navigation to the parent menu. Used to go one menu up.
         * Use this as a handler for navigating one level up in the menu.
         */
        void GoToParent();

        /**
         * Updates the selected index. If there is no next item it will not update anything.
         * Use this as a handler for navigating downwards in the menu.
         */
        void SelectNext();

        /**
         * Updates the selected index. If there is no previous item it will not update anything.
         * Use this as a handler for navigating upwards in the menu.
         */
        void SelectPrevious();

        /**
         * Executes the currently selected items action (calling cb or entering sub menu).
         * Use this as a handler for navigating the menu (for instance "clicking" an item).
         */
        void ExecuteItem();


        /**
         * Renders the menu to the screen.
         * A normal use case would be to create the menu, add menus to it.
         * Then bind listeners to the menu controls (e.g. buttons or joystick)
         * and render the menu when you want to display it.
         */
        void render();
    };
}