// Copyright (c) bis83. Distributed under the MIT License.

class Scene {
    constructor() {
        this.sceneNo = 0;
        this.reset();
    }

    onload(input, sprite) {
        sprite.load("img/title.png");
        sprite.load("img/title_back.png");
        sprite.load("img/back.png");
        sprite.load("img/back2.png");
        sprite.load("img/frame.png");
        sprite.load("img/player.png");
        sprite.load("img/shoot.png");
        sprite.load("img/gate.png");
        sprite.load("img/tama.png");
        sprite.load("img/number.png");
        sprite.load("img/time.png");
        sprite.load("img/gameover.png");
        sprite.load("img/clear.png");
    }

    reset() {
        this.time = 0;
    }

    update(input) {
        if(this.sceneNo == 0) {
            this.updateTitle(input);
        }
    }

    updateTitle(input) {
        this.time += 1;
        if(this.time >= 240) {
            this.time -= 240;
        }
    }

    draw(sprite) {
        if(this.sceneNo == 0) {
            this.drawTitle(sprite);
        }
    }

    drawTitle(sprite) {
        sprite.texture("img/title_back.png");
        sprite.uv(0, 0, 1, 1);
        sprite.trans(320, 240, 640, 480, 0);
        sprite.color(1, 1, 1, 1);
        sprite.draw();

        sprite.texture("img/title.png");
        sprite.uv(0, 0, 1, 1);
        sprite.trans(320, 300 + 10 * Math.sin(glMatrix.toRadian(360 * this.time / 240.0)), 640, 240, 0);
        sprite.color(1, 1, 1, 1);
        sprite.draw();
    }
}