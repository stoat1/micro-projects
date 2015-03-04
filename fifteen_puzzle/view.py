

import pygame
from pygame import Surface, Rect

import logging

class UnfocusedTileView:

    color = (0, 200, 0)
    thickness = 1

    def __init__(self, (x, y), size, digit):
        self.rect = Rect(x, y, size, size)
        self.text_surface = self.get_text_surf(str(digit))

    def get_text_surf(self, text):
        font = pygame.font.Font("freesansbold.ttf", 32)
        return font.render(text, False, (0, 0, 200))

    def draw(self, surface):
        pygame.draw.rect(surface, self.color, self.rect, self.thickness)
        rect = self.text_surface.get_rect()
        rect.center = self.rect.center
        surface.blit(self.text_surface, rect)

class FocusedTileView:

    color = (0, 255, 0)
    thickness = 2

    def __init__(self, (x, y), size, digit):
        self.rect = Rect(x, y, size, size)
        self.text_surface = self.get_text_surf(str(digit))

    def get_text_surf(self, text):
        font = pygame.font.Font("freesansbold.ttf", 32)
        return font.render(text, False, (0, 0, 255))
    
    def draw(self, surface):
        pygame.draw.rect(surface, self.color, self.rect, self.thickness)
        rect = self.text_surface.get_rect()
        rect.center = self.rect.center
        surface.blit(self.text_surface, rect)

class Tile:

    def __init__(self, (x, y), size, digit, (logic_x, logic_y)):
        self.logic_pos = (logic_x, logic_y)
        self.rect = Rect(x, y, size, size)
        self.focused_view = FocusedTileView((x, y), size, digit)
        self.unfocused_view = UnfocusedTileView((x, y), size, digit)

    def draw(self, surface, focused=False):
        if focused:
            self.focused_view.draw(surface)
        else:
            self.unfocused_view.draw(surface)


class View:

    offset = 32
    padding = 16
    tile_size = 64

    def __init__(self):
        self.rect = Rect(0, 0, 0, 0)
        self.cursor = (0, 0)
        self.tiles = []
        self.model = None
        self.focus = None

    def get_rect(self):
        return self.rect

    def update(self):
      
        (model_w, model_h) = self.model.get_dimension()
        width = (2 * View.offset + 
            (model_w - 1) * View.padding + 
            model_w * View.tile_size)
        height = (2 * View.offset + 
            (model_h - 1) * View.padding + 
            model_h * View.tile_size)

        self.rect.width, self.rect.height = width, height

        # Create new tile list
        self.tiles = []
        for x_pos in range(model_w):
            for y_pos in range(model_h):

                x = View.offset + x_pos * View.tile_size
                if x_pos:
                    x += View.padding * x_pos

                y = View.offset + y_pos * View.tile_size
                if y_pos:
                    y += View.padding * y_pos

                digit = self.model[(x_pos, y_pos)]
                if digit != None:
                    real_pos = (x, y)
                    logic_pos = (x_pos, y_pos)
                    new_tile = Tile(real_pos, View.tile_size, digit, logic_pos)
                    self.tiles.append(new_tile)

    def draw(self, surface):
        for tile in self.tiles:
            if (tile == self.focus):
                f = True
            else:
                f = False

            tile.draw(surface, focused=f)

        self.draw_cursor(surface)

    def draw_cursor(self, surface):
        pass

    def set_cursor(self, pos):
        self.cursor = pos
        self.focus = self.get_focus_tile()

    def click(self):
        if self.focus:
            self.model.touch_tile(self.focus.logic_pos)

    def get_focus_tile(self):
        for tile in reversed(self.tiles):
            if tile.rect.collidepoint(self.cursor):
                return tile


