package dataaccess

import (
	"errors"
	"example/gin/core/models"
)

type PairDAL struct {
	Source       map[int]models.Pair // this will be our in-memory "database"
	CurrentMaxId int
}

func (p *PairDAL) Get(id int) (models.Pair, error) {
	item, isExists := p.Source[id]
	if !isExists {
		return models.Pair{}, errors.New("Item not found")
	}

	return item, nil
}

func (p *PairDAL) GetAll() []models.Pair {
	var pairs []models.Pair
	for _, pair := range p.Source {
		pairs = append(pairs, pair)
	}
	return pairs
}

func (p *PairDAL) Save(pair models.Pair) {
	p.CurrentMaxId++
	pair.Id = p.CurrentMaxId
	p.Source[pair.Id] = pair
}

func (p *PairDAL) Update(id int, pair models.Pair) error {
	oldItem, isExists := p.Source[id]
	if !isExists {
		return errors.New("Item not found")
	}

	pair.Id = oldItem.Id
	p.Source[id] = pair
	return nil
}

func (p *PairDAL) Delete(id int) error {
	_, isExists := p.Source[id]
	if !isExists {
		return errors.New("Item not found")
	}
	delete(p.Source, id)
	return nil
}
