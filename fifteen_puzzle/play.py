#!/usr/bin/python

import pygame, sys
from pygame.locals import *
from model import Model
from view import View

import logging

class NotifyingModel(Model):

    def __init__(self, (w, h), view):
        Model.__init__(self, (w, h))
        self.view = view

    def success(self):
        logging.info('motion success')
        self.view.update()

    def failure(self):
        logging.info('motion failure')

def main():

    pygame.init()
    fpsClock = pygame.time.Clock()

    # Create view
    view = View()

    # Create model
    model = NotifyingModel((4, 4), view)

    # First update view
    view.model = model
    view.update()

    # Create window
    rect = view.get_rect()
    screen = pygame.display.set_mode((rect.width, rect.height))
    
    while True:

        screen.fill((0, 0, 0))

        # Draw game frame
        view.draw(screen)

        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == MOUSEMOTION:
                view.set_cursor(event.pos)
            elif event.type == MOUSEBUTTONDOWN:
                view.click()
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    pygame.event.post(pygame.event.Event(QUIT))

        pygame.display.update()
        fpsClock.tick(30)

if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG) 
    main()
