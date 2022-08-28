using Application.Interfaces;
using Domain;
using Domain.Entities;
using MediatR;

namespace Application.UseCases.Pairs.SavePair;

public record SavePairRequest(Pair Pair): IRequest<SavePairResponse>, IRequestValidator
{
    public void Validate()
    {
        if (string.IsNullOrWhiteSpace(Pair.Name))
        {
            throw new System.ArgumentException(StringResources.Name_Can_Not_Be_Empty);
        }
        
        if (string.IsNullOrWhiteSpace(Pair.Value))
        {
            throw new System.ArgumentException(StringResources.Value_Can_Not_Be_Empty);
        }
    }
}